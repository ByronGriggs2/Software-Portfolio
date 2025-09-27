#include "pch.h"
#include "PhysicsSimulation.h"
#include "ScenarioManager.h"
#include "SharedMemory.h"
#include "NetCompatibleGameObject.h"

using namespace std;
using namespace helpers;

PhysicsSimulation::PhysicsSimulation() {
	_myColor = undef;
}

void PhysicsSimulation::simLoop() {
	static auto scenarioManager = ScenarioManager::getInstance();
	QueryPerformanceCounter(&_start);
	bool tempGravity = _ImGuiState._gravityOn;
	bool tempInvert = _ImGuiState._gravityInverted;
	getInputs();
	if (_physicsReset) {
		scenarioManager->swapScene(_ImGuiState._currentScenarioIndex, _ImGuiState._gravityInverted);
		_physicsBufferStorage.clear();
		_physicsResetThisLoop = true;
		_renderReset |= true;
	}
	if (!_ImGuiState._paused) {
		scenarioManager->onUpdate(_ImGuiState._timestep, _ImGuiState._integrationType, _ImGuiState._gravityOn, _ImGuiState._gravityInverted);
	}
	endOfFrame();
	sleepUntilNextFrame();
}

void PhysicsSimulation::initialize() {
	CComPtr<ID3D11Device> tempDevice = nullptr;
	auto memory = SharedMemory::getInstance();
	{
		std::lock_guard<std::mutex> lock(memory->_mx);
		_myColor = memory->_simInit._peerColor;
		tempDevice = memory->_simInit._device;
	}
	QueryPerformanceFrequency(&_frequency);
	auto scenarioManager = ScenarioManager::getInstance();
	scenarioManager->initializeScenarioList(tempDevice, _myColor);

	scenarioManager->swapScene(_ImGuiState._currentScenarioIndex, false);
	_renderReset |= true;
	endOfFrame();
}

// Copy shared memory into local cache
void PhysicsSimulation::getInputs() {
	static auto memory = SharedMemory::getInstance();
	lock_guard<mutex> lock(memory->_mx);
	_physicsReset = memory->_physicsReset;
	_renderReset = false;

	_ImGuiState = memory->_ImGuiState;
}

// Write to shared memory. Not all of local cache is written.
void PhysicsSimulation::endOfFrame() {
	static auto scenarioManager = ScenarioManager::getInstance();
	static auto memory = SharedMemory::getInstance();
	//calculate early to not hog lock
	const auto temp = getNetCompatibleObjects();
	lock_guard lock(memory->_mx);
	if (_physicsResetThisLoop) {
		_physicsResetThisLoop = false;
		memory->_physicsReset = false;
	}
	memory->_renderReset |= _renderReset;
	memory->_actualSimulationSpeed = _actualSimulationSpeed;
	memory->_cameraPosition = scenarioManager->getCameraPos();
	memory->_lightPosition = scenarioManager->getLightPos();
	memory->setbackgroundColor(scenarioManager->getBackgroundColor());
	// This thread is the only thread allowed to modify this element (not enforced)
	memory->_globalObjects[_myColor - 1] = temp;
}

// Enforce target framerate and record actual framerate
void PhysicsSimulation::sleepUntilNextFrame() {
	QueryPerformanceCounter(&_end);
	const float targetTime = 1.0f / _ImGuiState._targetSimulationFrequency;
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

	_actualSimulationSpeed = 1.0f / newTime;
}

float PhysicsSimulation::getGravity() const {
	if (!_ImGuiState._gravityOn)
		return 0.0f;
	if (_ImGuiState._gravityInverted)
		return -1.0f * GRAVITY;
	return GRAVITY;
}

// Note that after this function returns, the vector of NetCompatibleGameObjects is atomically written
// to shared memory, but not the buffer storage. The NetCompatibleGameObjects have shared ptrs pointing
// to the buffers stored here.
std::vector<GlobalObjectStruct> PhysicsSimulation::getNetCompatibleObjects() {
	static auto scenarioManager = ScenarioManager::getInstance();
	return scenarioManager->getNetCompatibleObjects(_physicsBufferStorage);
}