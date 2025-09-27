#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include "DirectXMath.h"
#include "NetCompatibleImGuiState.h"

namespace helpers {
	enum peerColor;
	enum IntegrationType;
	struct shaderListStruct;
	struct ObjectPtrs;
	struct RenderedObjectStruct;
}
class NetCompatibleGameObject;
struct GlobalObjectStruct;

// For each thread loop, the thread is expected to lock ALL of the shared memory, copy to its cache, unlock, do calculations, lock, and write to cache (and unlock)
class SharedMemory final {
	SharedMemory();
public:
	SharedMemory(SharedMemory&) = delete;
	SharedMemory(SharedMemory&&) = delete;
	SharedMemory& operator=(const SharedMemory&) = delete;
	SharedMemory& operator=(const SharedMemory&&) = delete;
	static std::shared_ptr<SharedMemory> getInstance() {
		static std::shared_ptr<SharedMemory> instance(new SharedMemory);
		return instance;
	}
	std::mutex _mx;

	// This is prepared by the render and network loops and used to initilize the physics loop.
	struct SimulationInitStruct {
		// Supposedly the device is threadsafe
		CComPtr <ID3D11Device> _device;
		std::atomic<bool> _renderDone = false;
		helpers::peerColor _peerColor;
		std::atomic<bool> _networkDone = false;
	};
	SimulationInitStruct _simInit;

// Reset controls
// These may only be set to false by the thread that performs the reset
	// This indicates that a reset was input on the local machine
		// only network may set this to false (not enforced)
	bool _resetPeers = false;

	// These are for resetting individual threads locally
		// only render may set this to false (not enforced). Also, only physics may set it to true
	bool _renderReset = false;
		// only physics may set this to false (not enforced)
	bool _physicsReset = false;
		// only network may set this to false (not enforced)
	bool _networkReset = false;
/////////
 
	////////////////////////
	// These members are organized by what threads need to copy them at the start of their loop
	////////////////////////
	// Physics thread (none)

	// Render thread
	float _actualSimulationSpeed = 200.0f;
	float _actualNetworkSpeed = 20.0f;
	DirectX::XMVECTOR _cameraPosition = DirectX::XMVectorSet(0, 1.0f, -1.5f, 1.0f);
	DirectX::XMVECTOR _lightPosition = DirectX::XMVectorSet(0, 1000, 0, 1);
private:
	float _backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
public:
	void getBackgroundColor(std::shared_ptr<float[]>& colorToSet) const {
		for (auto i = 0; i < 4; i++) {
			colorToSet.get()[i] = _backgroundColor[i];
		}
	}
	void setbackgroundColor(const float* backgroundColor) {
		for (auto i = 0; i < 4; i++) {
			_backgroundColor[i] = backgroundColor[i];
		}
	}

	// Network thread (none)

	// All 3 thread types
	NetCompatibleImGuiState _ImGuiState;

	// Red, yellow, green, blue
	// The physics thread can read and write this computer's color. It doesn't need to see the other colors.
	// The render and network thread can only read all 4 colors
	//ObjectColorStruct _objectData[4];
	std::vector<GlobalObjectStruct> _globalObjects[4] = {};
};