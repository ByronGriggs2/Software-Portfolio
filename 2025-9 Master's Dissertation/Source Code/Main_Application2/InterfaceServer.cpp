#include "pch.h"
#include "InterfaceServer.h"
#include "definitions.h"
#include "MLP.h"
#include "helpers.h"
#include "ReplayBuffer.h"

using namespace helpers;
using std::mutex; using std::lock_guard, std::cout, std::endl;

int InterfaceServer::initialize() {
	static_assert(sizeof(InputsPacket) == 11, "InputsPacket must be 11 bytes");
	static_assert(sizeof(StatePacket) == 254);

	// Set up pipe
	LPCWSTR pipeName = L"\\\\.\\pipe\\AwariaServerToClientPipe";
	_pipeServerToClient = CreateNamedPipeW(
		pipeName,
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_BYTE |
		PIPE_READMODE_BYTE |
		PIPE_WAIT, // blocking
		1, // 1 instance
		sizeof(InputsPacket),
		sizeof(InputsPacket),
		0, // 50 ms
		nullptr
	);
	if (_pipeServerToClient == INVALID_HANDLE_VALUE)
		return -1;
	pipeName = L"\\\\.\\pipe\\AwariaClientToServerPipe";
	_pipeClientToServer = CreateNamedPipeW(
		pipeName,
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_BYTE |
		PIPE_READMODE_BYTE |
		PIPE_WAIT, // blocking
		1, // 1 instance
		sizeof(StatePacket),
		sizeof(StatePacket),
		0, // 50 ms
		nullptr
	);
	if (_pipeClientToServer == INVALID_HANDLE_VALUE)
		return -1;
	return 0;
}
int InterfaceServer::start() {
#ifdef _DEBUG
	_stateLogStream.open("Game State Log.txt");
	_detailedStateLogStream.open("Game states received log.txt");
#endif

	std::cout << "Waiting for client to connect" << std::endl;
	BOOL connected = ConnectNamedPipe(_pipeServerToClient, nullptr) ? TRUE : GetLastError() == ERROR_PIPE_CONNECTED;
	if (!connected) {
		CloseHandle(_pipeServerToClient);
		return -1;
	}
	connected = ConnectNamedPipe(_pipeClientToServer, nullptr) ? TRUE : GetLastError() == ERROR_PIPE_CONNECTED;
	if (!connected) {
		CloseHandle(_pipeClientToServer);
		return -1;
	}
	std::cout << "Client connected" << std::endl;

	// set up threads
	_terminateThreads.store(false);
	_transcvThread = std::thread(&InterfaceServer::transcvTask, this);
	DuplicateHandle(GetCurrentProcess(), (HANDLE)_transcvThread.native_handle(), GetCurrentProcess(), &_transcvThreadHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	_watchdog = std::thread(&InterfaceServer::watchdogTask, this);
	return 0;
}
InterfaceServer::~InterfaceServer() {
	_terminateThreads.store(true);

	if (_watchdog.joinable())
		_watchdog.join();
	if (_transcvThreadHandle) {
		if (_transcvThread.joinable()) {
			CancelSynchronousIo(_transcvThreadHandle);
			_transcvThread.join();
		}
		CloseHandle(_transcvThreadHandle);
		_transcvThreadHandle = nullptr;
	}

	if (_pipeServerToClient) {
		DisconnectNamedPipe(_pipeServerToClient);
		CloseHandle(_pipeServerToClient);
		_pipeServerToClient = nullptr;
	}
	if (_pipeClientToServer) {
		DisconnectNamedPipe(_pipeClientToServer);
		CloseHandle(_pipeClientToServer);
		_pipeClientToServer = nullptr;
	}
}
void InterfaceServer::defaultStartup() {
	start();
	// Wait to firmly enter main menu
	Sleep(1000);
	// Enter level select
	sendAction(interact);
	Sleep(50);
	sendAction(noop);
	Sleep(250);
}

///////////////////////// Mode switching
void InterfaceServer::setupDirectResponse(MLP* DQN, ReplayBuffer* buffer) {
	_DQN = DQN;
	_buffer = buffer;
}
void InterfaceServer::activateDirectResponse() {
	if (_DQN == nullptr) {
		throw std::runtime_error("Direct response not set up");
		return;
	}
	if (_mode == training) {
		cout << "####WARNING: already in training mode" << endl;
		return;
	}
	{
		lock_guard<mutex> lock(_threadInputLock);
		_mode = training;
	}
	_threadOK.wait(false);
	_threadOK.store(false);
}
void InterfaceServer::deactivateDirectResponse() {
	if (_mode == normal) {
		cout << "####WARNING: already in normal mode" << endl;
		return;
	}
	{
		lock_guard<mutex> lock(_threadInputLock);
		_mode = normal;
	}
	_threadOK.wait(false);
	_threadOK.store(false);
}
////////////////////// Default mode
bool InterfaceServer::getGameState(StatePacket& state) {
	bool stateIsNew;
	{
		std::lock_guard<std::mutex> lock(_recvLock);
		state = _mostRecentGamestate;
		stateIsNew = _newGameState;
		_newGameState = false;
	}
#ifdef _DEBUG
	_stateLogStream << state;
#endif
	return stateIsNew;
}
bool InterfaceServer::getPlayerDeathFlag(bool reset) {
	std::lock_guard<std::mutex> lock(_recvLock);
	bool temp = _playerDeathFlag;
	if (reset)
		_playerDeathFlag = false;
	return temp;
}
bool InterfaceServer::getPlayerWinFlag(bool reset) {
	std::lock_guard<std::mutex> lock(_recvLock);
	bool temp = _playerWinFlag;
	if (reset)
		_playerWinFlag = false;
	return temp;
}
void InterfaceServer::getGameStateToConsole() {
	StatePacket state;
	{
		std::lock_guard<std::mutex> lock(_recvLock);
		if (!_newGameState)
			return;
		state = _mostRecentGamestate;
		_newGameState = false;
	}
	StatePacket::printToConsole(state);
}
void InterfaceServer::sendAction(helpers::playerAction actionToSend) {
	std::lock_guard<std::mutex> lock(_threadInputLock);
	_actionToSend = actionToSend;
	_sendReady = true;
}
std::string InterfaceServer::sendActionAsPacket(playerAction action) {
	InputsPacket currentPacket;
	switch (action) {
	case noop:
		break;
	case moveNorth:
		currentPacket._y = 1.0f;
		break;
	case moveNortheast:
		currentPacket._x = 1.0f;
		currentPacket._y = 1.0f;
		break;
	case moveEast:
		currentPacket._x = 1.0f;
		break;
	case moveSoutheast:
		currentPacket._x = 1.0f;
		currentPacket._y = -1.0f;
		break;
	case moveSouth:
		currentPacket._y = -1.0f;
		break;
	case moveSouthwest:
		currentPacket._x = -1.0f;
		currentPacket._y = -1.0f;
		break;
	case moveWest:
		currentPacket._x = -1.0f;
		break;
	case moveNorthwest:
		currentPacket._x = -1.0f;
		currentPacket._y = 1.0f;
		break;
	case dashNorth:
		currentPacket._y = 1.0f;
		currentPacket._secondary = 1;
		break;
	case dashNorthwest:
		currentPacket._y = 1.0f;
		currentPacket._x = -1.0f;
		currentPacket._secondary = 1;
		break;
	case dashWest:
		currentPacket._x = -1.0f;
		currentPacket._secondary = 1;
		break;
	case dashSouthwest:
		currentPacket._x = -1.0f;
		currentPacket._y = -1.0f;
		currentPacket._secondary = 1;
		break;
	case dashSouth:
		currentPacket._y = -1.0f;
		currentPacket._secondary = 1;
		break;
	case dashSoutheast:
		currentPacket._x = 1.0f;
		currentPacket._y = -1.0f;
		currentPacket._secondary = 1;
		break;
	case dashEast:
		currentPacket._x = 1.0f;
		currentPacket._secondary = 1;
		break;
	case dashNortheast:
		currentPacket._x = 1.0f;
		currentPacket._y = 1.0f;
		currentPacket._secondary = 1;
		break;
	case interact:
		currentPacket._primary = 1;
		break;
	case cancel:
		currentPacket._cancel = 1;
		break;
	default:
		return "Invalid player action";
		break;
	}
	return sendPacket(currentPacket);
}
std::string InterfaceServer::sendPacket(const InputsPacket& packet) {
	DWORD bytesWritten = 0;
	if (_terminateThreads.load())
		return "OK";
	BOOL ok = WriteFile(
		_pipeServerToClient,
		&packet,
		sizeof(packet),
		&bytesWritten,
		nullptr
	);
	_threadUnblocked.store(true);
	return (ok == TRUE) ? "OK" : "Failed to write packet";
}
/////////////////////// Training mode
Transition InterfaceServer::getOldestTransition(bool remove) {
	std::unique_lock<mutex> lock(_recvLock);
	lock.unlock();
	while (true) {
		lock.lock();
		if (!_transitionBuffer.empty())
			break;
		lock.unlock();
		Sleep(static_cast<DWORD>(0.25 * ONEFRAMEMS));
	}
	Transition temp = _transitionBuffer.front();
	if (remove)
		_transitionBuffer.erase(_transitionBuffer.begin());
	lock.unlock();
	return temp;
}
Transition InterfaceServer::getNewestTransition(bool remove, bool expectOneFrame) {
	std::unique_lock<mutex> lock(_recvLock);
	lock.unlock();
	while (true) {
		lock.lock();
		if (!_transitionBuffer.empty())
			break;
		lock.unlock();
		Sleep(static_cast<DWORD>(0.25 * ONEFRAMEMS));
	}
	if (expectOneFrame && _transitionBuffer.size() > 1) {
		printSystemTime();
		std::cout << "####WARNING: getNewestTransition was not called last frame" << std::endl;
	}
	Transition temp = _transitionBuffer.back();
	if (remove)
		_transitionBuffer.pop_back();
	lock.unlock();
	return temp;
}
//bool InterfaceServer::getMostRecentDeathTransition(Transition& trans, bool removeItem) {
//	std::lock_guard<mutex> lock(_recvLock);
//	if (!_playerDeathFlag)
//		return false;
//	_playerDeathFlag = false;
//	for (int i = static_cast<int>(_transitionBuffer.size()) - 1; i > -1; i--) {
//		if (_transitionBuffer[i]._S1.getPlayerDeath()) {
//			trans = _transitionBuffer[i];
//			if (removeItem)
//				_transitionBuffer.erase(_transitionBuffer.begin() + i);
//			return true;
//		}
//	}
//	cout << "####ERROR: No player death found." << endl;
//	cout << "***************************" << endl;
//	for (const auto& trans : _transitionBuffer)
//		trans.printToConsole();
//	cout << "***************************" << endl;
//	return false;
//}
//bool InterfaceServer::getMostRecentWinTransition(Transition& trans, bool removeItem) {
//	std::lock_guard<mutex> lock(_recvLock);
//	if (!_playerWinFlag)
//		return false;
//	_playerWinFlag = false;
//	for (int i = static_cast<int>(_transitionBuffer.size()) - 1; i > -1; i--) {
//		if (_transitionBuffer[i]._S0.getCurrentLevel() != 0 && _transitionBuffer[i]._S1.getCurrentLevel() == 0) {
//			trans = _transitionBuffer[i];
//			if (removeItem)
//				_transitionBuffer.erase(_transitionBuffer.begin() + i);
//			return true;
//		}
//	}
//	cout << "####ERROR: No player win found." << endl;
//	return false;
//}
//////////////////////////// Extra thread (communication thread)
void InterfaceServer::transcvTask() {
	threadMode currentMode = normal;
	StatePacket lastFrameGamestate;
	playerAction lastFrameAction = noop;
	bool lastReadUnsuccessful = false;
	while (!_terminateThreads.load()) {
		bool firstTrainingFrame = false;
		float localEpsilon;
		// Atomically copy inputs and use them for the duration of the loop
		playerAction actionToSend;
		{
			{
				lock_guard<mutex> lock(_threadInputLock);
				if (currentMode != _mode && _mode == normal) {
					currentMode = _mode;
					_threadOK.store(true);
					_threadOK.notify_all();
				}
				else if (currentMode != _mode && _mode == training) {
					firstTrainingFrame = true;
					{
						lock_guard<mutex> lock(_recvLock);
						_transitionBuffer.clear();
					}
					currentMode = _mode;
					_threadOK.store(true);
					_threadOK.notify_all();
				}
				localEpsilon = _epsilonCopy.load();
			}
			// If the action is interact (primary) or cancel, the button is pressed once. If the action is any movement action, the buttons are pressed every frame.
			// The Unity engine interprets this as repeated presses for digital (primary, secondary, cancel) buttons and continuous presses for analog(movement) buttons.
			// Be aware that when you send a dash action you're hammering the dash button every frame until you send noop. Otherwise it should work intuitively.
			if (currentMode == normal) {
				switch (_actionToSend) {
				case noop:
				case interact:
				case cancel:
					actionToSend = _sendReady ? _actionToSend : noop;
					break;
				default:
					actionToSend = _sendReady ? _actionToSend : lastFrameAction;
					break;
				}
				_sendReady = false;
			}
		}
		StatePacket thisFrameGamestate;
		// Read game state
		{
			if (!blockingReadGamestate(thisFrameGamestate)) {
				printSystemTime();
				cout << "####WARNING: Unsuccessful read" << endl;
				lastReadUnsuccessful = true;
			}
			else
				lastReadUnsuccessful = false;
		}
		// Calculate reaction
		{
			if (currentMode == training) {
				actionToSend = _DQN->getActionAtomic(thisFrameGamestate, localEpsilon);
			}
			// Already done
			else if (currentMode == normal) {}
		}
		// Reply with action
		{
			if (sendActionAsPacket(actionToSend) != "OK") {
				printSystemTime();
				std::cout << "####WARNING: Unsuccessful write" << endl;
			}
		}
		// Save game state data
		{
			lock_guard<mutex> lock(_recvLock);
			if (currentMode == training && !firstTrainingFrame && !lastReadUnsuccessful) {
				Transition temp;
				temp._S0 = lastFrameGamestate;
				temp._A0 = lastFrameAction;
				temp._S1 = thisFrameGamestate;
				// valid is set true later when _R1 and _F are set
				temp._valid = false;
				if (_transitionBuffer.size() > 10) {
					printSystemTime();
					std::cout << "####WARNING: transition buffer overfilled" << std::endl;
					_transitionBuffer.erase(_transitionBuffer.begin());
				}
				_transitionBuffer.push_back(temp);
			}
			else if (currentMode == normal) {
				_mostRecentGamestate = thisFrameGamestate;
				_newGameState = true;
			}
			_playerDeathFlag |= thisFrameGamestate.getPlayerDeath();
			if (!_transitionBuffer.empty())
				_playerWinFlag |= currentMode == training && _transitionBuffer.back()._S0.getCurrentLevel() != 0 && _transitionBuffer.back()._S1.getCurrentLevel() == 0;
			_recvCounter++;
		}
		lastFrameGamestate = thisFrameGamestate;
		lastFrameAction = actionToSend;
#ifdef _DEBUG
		_detailedStateLogStream << thisFrameGamestate;
#endif
	}
}
bool InterfaceServer::blockingReadGamestate(StatePacket& gamestate) {
	DWORD bytesRead = 0;
	bool success = false;
	if (_terminateThreads.load())
		return false;
	if (ReadFile(_pipeClientToServer, &gamestate, sizeof(StatePacket), &bytesRead, nullptr)) {
		success = true;
	}
	_threadUnblocked.store(true);
	return success;
}
int InterfaceServer::getRecvCounter() {
	lock_guard<mutex> lock(_recvLock);
	return _recvCounter;
}
bool InterfaceServer::newData() {
	lock_guard<mutex> lock(_recvLock);
	bool ret = _newGameState;
	_newGameState = false;
	return ret;
}
void InterfaceServer::stopComms(bool closeConnection) {
	_terminateThreads.store(true);

	if (_watchdog.joinable())
		_watchdog.join();
	if (_transcvThreadHandle) {
		if (_transcvThread.joinable()) {
			CancelSynchronousIo(_transcvThreadHandle);
			_transcvThread.join();
		}
		CloseHandle(_transcvThreadHandle);
		_transcvThreadHandle = nullptr;
	}

	if (closeConnection) {
		if (_pipeServerToClient) {
			DisconnectNamedPipe(_pipeServerToClient);
			CloseHandle(_pipeServerToClient);
			_pipeServerToClient = nullptr;
		}
		if (_pipeClientToServer) {
			DisconnectNamedPipe(_pipeClientToServer);
			CloseHandle(_pipeClientToServer);
			_pipeClientToServer = nullptr;
		}
	}
}
// Maybe move watchdog up the hierarchy as its own class if not the entire server
void InterfaceServer::watchdogTask() {
	// Wait for communication to begin
	while (!_threadUnblocked.load())
		Sleep(200);
	// Ehhh go ahead and wait another 5 seconds to be safe
	Sleep(5000);
	if (_buffer == nullptr) {
		printSystemTime();
		cout << "Direct response not set up. Watchdog exiting." << endl;
		Sleep(1000);
		return;
	}
	bool errorLastFrame = false;
	while (!_terminateThreads.load()) {
		if (!_threadUnblocked.load()) {
			if (!errorLastFrame) {
				printSystemTime();
#ifndef SAFE_TO_TAB_OUT
				std::cout << "####WARNING: Server communication has been blocking for 1 second or more. Restarting communication and deleting last second of data..." << endl << endl;
				{
					{
						lock_guard<mutex> lock(_recvLock);
						_transitionBuffer.clear();
					}
					_buffer->purgeLastSecond();
					//_DQN->handleErroneousTraining();
				}
#else
				std::cout << "####WARNING: Server communication has been blocking for 1 second or more. Restarting communication..." << endl << endl;
#endif
				errorLastFrame = true;
			}
			CancelSynchronousIo(_transcvThreadHandle);
			// See if thread has recovered
			_threadUnblocked.store(false);
			Sleep(1000);
			if (_threadUnblocked.load()) {
				printSystemTime();
				cout << "Recovered from error after forcing unblock" << endl;
				errorLastFrame = false;
				continue;
			}
			printSystemTime();
#ifdef WATCHDOG_DELETE
			std::cout << "Unblocking the server comm thread did not resolve the issue. Retrying without deleting further data..." << endl << endl;
#else
			std::cout << "Unblocking the server comm thread did not resolve the issue. Retrying..." << endl << endl;
#endif
			continue;
		}
		_threadUnblocked.store(false);
		Sleep(1000);
	}
	printSystemTime();
	std::cout << "Watchdog thread exiting" << endl;
}