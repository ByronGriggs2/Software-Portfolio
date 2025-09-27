#include "pch.h"
#include "NetworkManager.h"
#include "SharedMemory.h"
#ifndef NOT_DISTRIBUTED
#include "ImGuiState_generated.h"
#endif
using namespace helpers;

void NetworkManager::initialize() {
	auto memory = SharedMemory::getInstance();
	{
		std::lock_guard<std::mutex> lock(memory->_mx);
		_device = memory->_simInit._device;
	}
	WSADATA wsaData;
	// Create version identifier
	WORD wVersionRequested = MAKEWORD(2, 0);
	// Startup windows sockets
	if (WSAStartup(wVersionRequested, &wsaData)) {
#ifdef _DEBUG
		throw MyException("Server Socket initialisation failed");
#endif
		return;
	}
	_broadcaster = std::make_unique<UDPBroadcastSend>();
	_receiver = std::make_unique<UDPBroadcastReceive>();
	_TCPManager = std::make_unique<TCPPeer>();
	QueryPerformanceFrequency(&_frequency);
}

void NetworkManager::assignColorToComputer() {
#ifdef VERBOSE
	if (!_TCPManager->fullyConnected()) {
		std::cout << "I am not connected to all peers" << std::endl;
#ifdef _DEBUG
		throw ShouldNotRunException();
#endif
	}
#endif
	auto memory = SharedMemory::getInstance();
	std::lock_guard<std::mutex> lock(memory->_mx);
	memory->_simInit._peerColor = _TCPManager->getColor();
	memory->_simInit._networkDone = true;
}

// Returns true iff network has been established
bool NetworkManager::establishNetworkLoop() {
	// Accept any connection requests
	while (_TCPManager->acceptConnectionRequestIfNotConnected()) {
#ifdef VERBOSE
		std::cout << "I accepted a peer's connection request" << std::endl;
#endif
	if (finishNetworkLoop())
		return true;
	}
	// Look for broadcasts
	sockaddr_in otherAddress;
	const std::string message = _receiver->myReceive(otherAddress);
	if (message[0] != '\0' && message == "Byron's peer requesting connection") {
		otherAddress.sin_port = htons(LANLISTENING);
		const bool temp = _TCPManager->sendConnectionRequestIfNotConnected(otherAddress);
#ifdef VERBOSE
		if (temp)
			std::cout << "My connection request was accepted" << std::endl;
#endif
	}
	if (finishNetworkLoop())
		return true;
	// Send broadcast
	_broadcaster->broadcast("Byron's peer requesting connection");
	// Sleep to reduce network traffic I guess idk how that works
	Sleep(100);
	return false;
}

bool NetworkManager::finishNetworkLoop() {
	if (_TCPManager->fullyConnected()) {
		_TCPManager->assignColors();
		_broadcaster.reset();
		_receiver.reset();
#ifdef _DEBUG
		std::cout << "Connected to all peers" << std::endl;
#endif
		return true;
	}
	return false;
}

#ifdef NOT_DISTRIBUTED
void NetworkManager::communicationLoop() {
	QueryPerformanceCounter(&_start);
	getInputs();
	if (_networkReset) {
		_networkResetThisLoop = true;
	}
	if (_resetPeers) {
		_resetPeersThisLoop = true;
		endOfFrame();
		sleepUntilNextFrame();
		return;
	}
	endOfFrame();
	sleepUntilNextFrame();
	return;
}
#endif

void NetworkManager::endOfFrame() {
	auto memory = SharedMemory::getInstance();
	std::lock_guard<std::mutex> lock(memory->_mx);
	if (_resetPeersThisLoop) {
		_resetPeersThisLoop = false;
		memory->_resetPeers = false;
	}
	if (_networkResetThisLoop) {
		_networkResetThisLoop = false;
		memory->_networkReset = false;
	}
	memory->_actualNetworkSpeed = _actualNetworkSpeed;
	memory->_ImGuiState = _ImGuiState;

#ifndef NOT_DISTRIBUTED
	for (int i = 0; i < 4; i++) {
		if (i != _TCPManager->getColor() - 1) {
			memory->_globalObjects[i] = _globalObjects[i];
		}
	}
	memory->_physicsReset |= _physicsReset;
	memory->_networkReset |= _networkReset;
#endif
}

void NetworkManager::sleepUntilNextFrame() {
	QueryPerformanceCounter(&_end);
	const float targetTime = 1.0f / _ImGuiState._targetNetworkFrequency;
	const float time = (static_cast<float>((_end.QuadPart - _start.QuadPart) * 1.0f / _frequency.QuadPart));

	// Calculate sleep cycles in 15.6 ms chunks
	float netTime = 1000.0f * (targetTime - time);
	int sleepCycles = 0;
	while (netTime >= SLEEP_RESOLUTION) {
		netTime -= SLEEP_RESOLUTION;
		sleepCycles++;
	}

	// Sleep for most of remaining frame time
	const DWORD sleepTime = static_cast<DWORD>(round(SLEEP_RESOLUTION * sleepCycles));
	if (sleepTime != 0)
		Sleep(sleepTime);

	// Busy loop the remaining time
	float newTime;
	while (true) {
		QueryPerformanceCounter(&_end);
		newTime = (static_cast<float>((_end.QuadPart - _start.QuadPart) * 1.0f / _frequency.QuadPart));
		if (newTime >= targetTime)
			break;
	}

	_actualNetworkSpeed = 1.0f / newTime;
}

void NetworkManager::getInputs(){
	auto memory = SharedMemory::getInstance();
	std::lock_guard<std::mutex> lock(memory->_mx);
	_resetPeers = memory->_resetPeers;
	_networkReset = memory->_networkReset;
	_ImGuiState = memory->_ImGuiState;

#ifndef NOT_DISTRIBUTED
	_physicsReset = false;
	for (int i = 0; i < 4; i++) {
		if (i == _TCPManager->getColor() - 1) {
			_globalObjects[i] = memory->_globalObjects[i];
		}
		else {
			_globalObjects[i].clear();
		}
	}
#endif
}

#ifndef NOT_DISTRIBUTED
void NetworkManager::communicationLoop() {
	// Other than the frequency stuff, this loop's job is to get the positions of objects owned by other computers.
	// In order to closely follow the instructions, the positions of these objects must be gotten synchronously- that is, you cannot get the red positions twice before getting blue once.
	// In order to NOT limit the framerate of the application to the network framerate, this loop does not touch objects that are owned by this computer, and leaves that to physics.
	QueryPerformanceCounter(&_start);
	getInputs();
	/*
	1. Check if local wants me to reset myself
	2. Check if local wants me to reset remote (return)
	3. Receive read flatbuffers until receive buffer is empty
	4. Check if remote is trying to reset local (return)
	5. Send ImGuiData (buffer will be mostly emtpy if local has not changed it)
	6. Send Object data
	*/
	// Does not currently handle unexpected disconnects (or disconnects in general)
	if (_networkReset) {//1
#ifdef VERBOSE
		std::cout << "Network deleting its shared_ptr references to all yellow, green, blue objects" << std::endl;
#endif
		for (auto& coloredVector : _networkBufferStorage)
			coloredVector.clear();
		_networkResetThisLoop = true;
	}
	if (_resetPeers) {//2
		_ImGuiState._newData = true;
		_ImGuiState._resetLocal = true;
		sendImGuiState();
		_resetPeersThisLoop = true;
		_ImGuiState._newData = false;
		_ImGuiState._resetLocal = false;
		endOfFrame();
		Sleep(300);
		_TCPManager->clearRecvBuffers();
		return;
	}
	receiveAll();//3
	parseRemoteImGuis();
	if (_ImGuiState._resetLocal) { //4
		_physicsReset |= true;
		_networkReset |= true;
		_ImGuiState._resetLocal = false;
		endOfFrame();
		Sleep(300);
		_TCPManager->clearRecvBuffers();
		return;
	}
	parseRemoteGameObjects();
	sendImGuiState();//5
	sendObjects();//6
	endOfFrame();
	sleepUntilNextFrame();
}

void NetworkManager::receiveAll() {
	_loopGameObjects.clear();
	_loopImGuiStates.clear();
	for (auto i = 0; i < 4; i++) {
		const auto color = static_cast<peerColor>(i + 1);
		if (color == _TCPManager->getColor())
			continue;
		while (true) {
			const flatBufferType type = static_cast<flatBufferType>(_TCPManager->recvType(color));
			if (type == invalid)
				break;
			const int size = _TCPManager->recvSize(color);
			std::vector<uint8_t> tempBuffer(size);
			_TCPManager->recvFB(tempBuffer, color);

			if (type == imgui) {
				flatbuffers::Verifier verifier(tempBuffer.data(), tempBuffer.size());
				if (!verifier.VerifyBuffer<net::ImGuiState>())
					continue;
				_loopImGuiStates.push_back(tempBuffer);
				continue;
			}
			if (type == gameObject) {
				flatbuffers::Verifier verifier(tempBuffer.data(), tempBuffer.size());
				if (!verifier.VerifyBuffer<net::GameObject>())
					continue;
				_loopGameObjects.push_back(tempBuffer);
				continue;
			}
#ifdef _DEBUG
			throw ShouldNotRunException();
#endif;
		}
	}
}

void NetworkManager::parseRemoteImGuis() {
	for (auto& state : _loopImGuiStates) {
		const net::ImGuiState* recvState = flatbuffers::GetRoot<net::ImGuiState>(state.data());
		if (!recvState->new_data())
			continue;
		_ImGuiState = NetCompatibleImGuiState(recvState);
		_ImGuiState._dirty = true;
		return;
	}
}

void NetworkManager::parseRemoteGameObjects() {
	std::vector<int> seenIDs;
	for (auto& rawData : _loopGameObjects) {
		const net::GameObject* gameObj = flatbuffers::GetRoot<net::GameObject>(rawData.data());
		const int bufferID = gameObj->uid();
		if (std::find(seenIDs.begin(), seenIDs.end(), bufferID) != seenIDs.end()) {
			continue;
		}
		const peerColor color = static_cast<helpers::peerColor>(gameObj->color());
		auto foundID = _networkBufferStorage[color - 1].find(bufferID);
		if (foundID == _networkBufferStorage[color - 1].end()) {
			if (gameObj->buffer()->index_data()->empty() || gameObj->buffer()->vertex_data()->empty()) {
				continue;
			}
			_networkBufferStorage[color - 1][bufferID] = std::make_shared<NetCompatibleBuffers>(gameObj->buffer(), _device);
		}
		_globalObjects[color - 1].push_back({ false, NetCompatibleGameObject(gameObj, _networkBufferStorage[color - 1][bufferID]) });
		seenIDs.push_back(bufferID);
	}
	_loopGameObjects.clear();
}

void NetworkManager::sendObjects() {
	for (auto i = 0; i < 4; i++) {
		const auto myColor = _TCPManager->getColor();
		const auto otherColor = static_cast<peerColor>(i + 1);
		if (otherColor != myColor && _TCPManager->connectionAlive(otherColor)) {
			for (int j = 0; j < _globalObjects[myColor - 1].size(); j++) {
				const auto& currentObject = _globalObjects[myColor - 1][j];
				_TCPManager->sendObjectData(currentObject._object, currentObject._newItem, otherColor);
			}
		}
	}
}

void NetworkManager::sendImGuiState() {
	for (auto i = 0; i < 4; i++) {
		const auto myColor = _TCPManager->getColor();
		const auto otherColor = static_cast<peerColor>(i + 1);
		if (otherColor != myColor && _TCPManager->connectionAlive(otherColor)) {
			_TCPManager->sendImGuiState(_ImGuiState, otherColor);
		}
	}
}
#endif