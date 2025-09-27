#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include "definitions.h"
#include "SingletonReferences.h"

constexpr DWORD_PTR PROCESSOR0 = 1 << 0;
constexpr DWORD_PTR PROCESSOR1 = 1 << 1;
constexpr DWORD_PTR PROCESSOR2 = 1 << 2;

/////////////////////
// Singleton

class ThreadManager final {
	std::atomic<bool> _terminateThreads;
	std::thread _graphicsThread;
	std::thread _networkThread;
	std::vector<std::thread> _simulationThreads;
	
	void graphicsTask();
	void networkTask();
	void simulationTask(int processorID);

	SingletonReferences _ref;

	ThreadManager() = default;
public:
	ThreadManager(const ThreadManager& other) = delete;
	ThreadManager& operator=(const ThreadManager& other) = delete;
	ThreadManager(const ThreadManager&& other) = delete;
	ThreadManager& operator=(const ThreadManager&&) = delete;
	~ThreadManager();

	static std::shared_ptr<ThreadManager> getInstance() { 
		static std::shared_ptr<ThreadManager> instance(new ThreadManager);
		return instance;
	}

	// Allows us to close all threads when one thread stops. Possibly dangerous. Well I mean this is just a getter. The simulation.cpp does the actual dangerous part
	bool threadsActive() const {
		bool active = true;
		active &= _graphicsThread.joinable();
		active &= _networkThread.joinable();
		for (auto& myThread : _simulationThreads)
			active &= myThread.joinable();
		return active;
	}

	// AKA main thread
	void start();
};

