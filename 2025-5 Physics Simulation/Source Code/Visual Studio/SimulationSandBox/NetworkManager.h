#pragma once

#include <memory>
#include "UDPBroadcastSend.h"
#include "UDPBroadcastReceive.h"
#include "TCPPeer.h"
#include "SharedMemory.h"
#include "NetCompatibleGameObject.h"
#include "NetCompatibleImGuiState.h"
#include "atlbase.h"
#include "d3d11.h"

namespace helpers {
	enum peerColor;
}

/////////////////////
// Singleton

class NetworkManager final {
	// Local copy of shared memory
	CComPtr<ID3D11Device> _device = nullptr;
	bool _resetPeers = false;
	bool _networkReset = false;

	float _actualNetworkSpeed = 20.0f;
	NetCompatibleImGuiState _ImGuiState;

	// Internal variables
	bool _resetPeersThisLoop = false;
	bool _networkResetThisLoop = false;
	LARGE_INTEGER _start, _end, _frequency;

	std::unique_ptr<UDPBroadcastSend> _broadcaster = nullptr;
	std::unique_ptr<UDPBroadcastReceive> _receiver = nullptr;
	std::unique_ptr<TCPPeer> _TCPManager = nullptr;
	
#ifndef NOT_DISTRIBUTED
	bool _physicsReset = false;
	std::vector<GlobalObjectStruct> _globalObjects[4];
	/*This is the last bastion of storage for the vertex and index buffers received over TCP. There are references to them in a variety of places, but they are only deleted from here
on scenario reset or object destruction*/
// Note that 3 of these are filled by data received via TCP. The 4th one is filled by data from the physics thread.
	std::unordered_map<int, std::shared_ptr<NetCompatibleBuffers>> _networkBufferStorage[4];

	std::vector<std::vector<uint8_t>> _loopGameObjects;
	std::vector<std::vector<uint8_t>> _loopImGuiStates;
	enum flatBufferType {
		invalid,
		imgui,
		gameObject
	};
	void receiveAll();
	void parseRemoteImGuis();
	void parseRemoteGameObjects();
	void sendObjects();
	void sendImGuiState();
#endif

	NetworkManager() = default;
public:
	NetworkManager(const NetworkManager& other) = delete;
	NetworkManager& operator=(const NetworkManager& other) = delete;
	NetworkManager(const NetworkManager&& other) = delete;
	NetworkManager& operator=(const NetworkManager&&) = delete;
	static std::shared_ptr<NetworkManager> getInstance() {
		static std::shared_ptr<NetworkManager> instance(new NetworkManager);
		return instance;
	}
	~NetworkManager() { WSACleanup(); }

	void initialize();
	void communicationLoop();
	bool establishNetworkLoop();
	void assignColorToComputer();
private:
	bool finishNetworkLoop();
	void getInputs();
	void endOfFrame();
	void sleepUntilNextFrame();
};