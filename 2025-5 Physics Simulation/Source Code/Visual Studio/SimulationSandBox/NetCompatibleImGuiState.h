#pragma once

#include "DirectXMath.h"
#ifndef NOT_DISTRIBUTED
#include "ImGuiState_generated.h"
#endif

namespace helpers {
	enum IntegrationType;
	enum peerColor;
}

struct NetCompatibleImGuiState {
	// Render will not overwrite shared memory instance of this struct until
	// it has read it and done a full loop
	bool _dirty = false;
	// Data
	int _targetRenderFrequency = 60;
	int _targetSimulationFrequency = 200;
	int _targetNetworkFrequency = 20;
	int _currentScenarioIndex = 0;
	bool _paused = false;
	bool _gravityOn = true;
	bool _gravityInverted = false;
	float _timestep = 0.005f;
	float _backgroundColor[4] = { 0,0,0,0 };
	helpers::IntegrationType _integrationType;
	DirectX::XMVECTOR _cameraPositionOffset = DirectX::XMVectorZero();
	DirectX::XMMATRIX _cameraRotationMatrix = DirectX::XMMatrixIdentity();
	
	NetCompatibleImGuiState();

#ifndef NOT_DISTRIBUTED
	// Indicates that the receiving machine should update their ImGui data
	bool _newData = false;
	// Indicates that the receiving machine should reset their threads
	bool _resetLocal = false;

	NetCompatibleImGuiState(const net::ImGuiState*);
	net::Matrix4x4ImGui get4x4() const;
#endif
};