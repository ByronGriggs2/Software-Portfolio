#include "pch.h"
#include "ThreadManager.h"
#include "D3DFramework.h"
#include "NetworkManager.h"
#include "PhysicsSimulation.h"
#include "SharedMemory.h"

using std::lock_guard;
using std::mutex;
using namespace helpers;

void ThreadManager::start() {
	HANDLE mainHandle = GetCurrentThread();
	SetThreadAffinityMask(mainHandle, PROCESSOR0);

	_terminateThreads.store(false);
	_graphicsThread = std::thread(&ThreadManager::graphicsTask, this);
	//single thread for now
	_simulationThreads.push_back(std::thread(&ThreadManager::simulationTask, this, 3));
	_networkThread = std::thread(&ThreadManager::networkTask, this);
	// Once this function is exited, the main thread goes back to
	// Simulation.cpp and loops forever.
}

ThreadManager::~ThreadManager() {
	_terminateThreads.store(true);
	if (_graphicsThread.joinable())
		_graphicsThread.join();
	if (_networkThread.joinable())
		_networkThread.join();
	for (auto& thread : _simulationThreads)
		if (thread.joinable())
			thread.join();
}

void ThreadManager::graphicsTask() {
	HANDLE graphicsHandle = GetCurrentThread();
	SetThreadAffinityMask(graphicsHandle, PROCESSOR1);

	auto app = D3DFramework::getInstance();

	HRESULT HR = app->initDevice();
#ifdef _DEBUG
	if (HR < 0)
		throw MyException("D3DFramework initialization failed");
#endif
	while (!_terminateThreads.load()) {
		app->render();
	}
}

void ThreadManager::networkTask() {
	HANDLE networkHandle = GetCurrentThread();
	SetThreadAffinityMask(networkHandle, PROCESSOR2);
	auto memory = SharedMemory::getInstance();
#ifdef DISABLE_NETWORKING
	lock_guard<mutex> lock(memory->_mx);
	memory->_simInit._peerColor = red;
	memory->_simInit._networkDone = true;
	return;
#else
	auto networkManager = NetworkManager::getInstance();
	// Wait for device to be set
	while (!memory->_simInit._renderDone.load())
		Sleep(50);
	networkManager->initialize();
	while (true) {
		if (networkManager->establishNetworkLoop()) {
			networkManager->assignColorToComputer();
			break;
		}
		if (_terminateThreads.load())
			return;
	}
	
	while (!_terminateThreads.load()) {
		networkManager->communicationLoop();
	}
#endif
}

void ThreadManager::simulationTask(int processorID) {
	HANDLE myHandle = GetCurrentThread();
	SetThreadAffinityMask(myHandle, 1 << processorID);

	auto physics = PhysicsSimulation::getInstance();
	auto memory = SharedMemory::getInstance();

	while (!memory->_simInit._renderDone.load() || !memory->_simInit._networkDone.load()) {
		if (_terminateThreads.load())
			return;
		Sleep(50);
	}

	physics->initialize();

	while (!_terminateThreads.load()) {
		physics->simLoop();
	}
}