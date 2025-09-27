#pragma once
#include <windows.h>
#include <thread>
#include <mutex>
#include <vector>
#ifdef _DEBUG
#include <fstream>
#endif
#include "StatePacket.h"

namespace helpers {
	enum playerAction;
}
class MLP;
class ReplayBuffer;

class InterfaceServer
{
#pragma pack(push,1)
	struct InputsPacket {
		float _x = 0.0f;
		float _y = 0.0f;
		BYTE _primary = 0;
		BYTE _secondary = 0;
		BYTE _cancel = 0;
	};
#pragma pack(pop)
	// Thread and pipe management
	HANDLE _pipeServerToClient;
	HANDLE _pipeClientToServer;
	std::atomic<bool> _terminateThreads;
	std::thread _transcvThread;
	std::thread _watchdog;
	std::atomic<bool> _threadUnblocked{ false };
	HANDLE _transcvThreadHandle = nullptr;
	void transcvTask();
	void watchdogTask();
	// Input data for the thread
	std::mutex _threadInputLock;
	enum threadMode {
		normal,
		training
	};
	threadMode _mode = normal;
	bool _sendReady = false;
	helpers::playerAction _actionToSend;

	// Allows main thread to wait until extra thread is done switching
	std::atomic<bool> _threadOK{ false };

	// For neural network response mode
	MLP* _DQN = nullptr; // Owned by Control
	ReplayBuffer* _buffer = nullptr; // Owned by DeepQManager
	std::atomic<float> _epsilonCopy{ 1.0f }; // Actual value owned by control

	// Receive gamestate
	std::mutex _recvLock;
	bool blockingReadGamestate(StatePacket&);
	std::vector<Transition> _transitionBuffer; // Neural network response
	StatePacket _mostRecentGamestate; // Default response
	bool _newGameState = false; // "
	int _recvCounter = 0; // "
	bool _playerDeathFlag = false; // "
	bool _playerWinFlag = false; // "

	// Send action
	std::string sendActionAsPacket(helpers::playerAction);
	std::string sendPacket(const InputsPacket&);
#ifdef _DEBUG
	std::ofstream _stateLogStream;
	std::ofstream _detailedStateLogStream;
#endif
public:
	int initialize();
	int start();
	void defaultStartup();
	~InterfaceServer();

	void setupDirectResponse(MLP*, ReplayBuffer*);
	void activateDirectResponse();
	void deactivateDirectResponse();

	bool getGameState(StatePacket& state); // Default response
	void getGameStateToConsole(); // "
	bool newData(); // "
	void resetRecvCounter() { std::lock_guard<std::mutex> lock(_recvLock); _recvCounter = 0; } // "
	int getRecvCounter(); // "
	void sendAction(helpers::playerAction actionToSend); // "
	bool getPlayerDeathFlag(bool reset = false); // "
	bool getPlayerWinFlag(bool reset = false);
	Transition getOldestTransition(bool removeItem = false); // Neural network response
	Transition getNewestTransition(bool removeItem = false, bool expectOneFrame = false); // "
	//bool getMostRecentDeathTransition(Transition& trans, bool removeItem = false);
	//bool getMostRecentWinTransition(Transition& trans, bool removeItem = false);
	void setEpsilon(float epsilon) { _epsilonCopy.store(epsilon); }
	void stopComms(bool closeConnection = false);
};

