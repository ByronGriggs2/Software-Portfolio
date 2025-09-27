#pragma once

#include <DirectXMath.h>
#include "unordered_map"
#include "NetCompatibleImGuiState.h"

// Currently single threaded
namespace helpers {
	enum peerColor;
}
class NetCompatibleGameObject;
class NetCompatibleBuffers;
struct GlobalObjectStruct;

/////////////////////
// Singleton

class PhysicsSimulation final {
	// local copy of shared memory
	bool _physicsReset = false;
	bool _renderReset = false;

	NetCompatibleImGuiState _ImGuiState;
	float _actualSimulationSpeed = 200.0f;
	
	// Internal use
	helpers::peerColor _myColor;
	LARGE_INTEGER _start, _end, _frequency;
	bool _physicsResetThisLoop = false;

	// The network thread has one of these as well, but for all 4 colors. Notably, the network's
	// record for this computer's color is empty. The missing buffers are stored here, in the
	// physics thread
	std::unordered_map<int, std::shared_ptr<NetCompatibleBuffers>> _physicsBufferStorage;

	std::vector<GlobalObjectStruct> getNetCompatibleObjects();
	// Will not be a singleton in final version
	PhysicsSimulation();
public:
	PhysicsSimulation(const PhysicsSimulation& other) = delete;
	PhysicsSimulation& operator=(const PhysicsSimulation& other) = delete;
	PhysicsSimulation(const PhysicsSimulation&& other) = delete;
	PhysicsSimulation& operator=(const PhysicsSimulation&&) = delete;
	static std::shared_ptr<PhysicsSimulation> getInstance() {
		static std::shared_ptr<PhysicsSimulation> instance(new PhysicsSimulation);
		return instance;
	}

	void initialize();
	void simLoop();
	void getInputs();
	void endOfFrame();
	void sleepUntilNextFrame();

	float getGravity() const;
};