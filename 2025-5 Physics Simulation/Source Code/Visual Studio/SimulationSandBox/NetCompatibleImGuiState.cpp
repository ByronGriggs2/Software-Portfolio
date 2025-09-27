#include "pch.h"
#include "NetCompatibleImGuiState.h"

using namespace DirectX;

NetCompatibleImGuiState::NetCompatibleImGuiState() {
	_integrationType = helpers::IntegrationType::euler;
}

#ifndef NOT_DISTRIBUTED
net::Matrix4x4ImGui NetCompatibleImGuiState::get4x4() const {
	float matrixData[16] = {
		_cameraRotationMatrix.r[0].m128_f32[0], _cameraRotationMatrix.r[0].m128_f32[1], _cameraRotationMatrix.r[0].m128_f32[2], _cameraRotationMatrix.r[0].m128_f32[3],
		_cameraRotationMatrix.r[1].m128_f32[0], _cameraRotationMatrix.r[1].m128_f32[1], _cameraRotationMatrix.r[1].m128_f32[2], _cameraRotationMatrix.r[1].m128_f32[3],
		_cameraRotationMatrix.r[2].m128_f32[0], _cameraRotationMatrix.r[2].m128_f32[1], _cameraRotationMatrix.r[2].m128_f32[2], _cameraRotationMatrix.r[2].m128_f32[3],
		_cameraRotationMatrix.r[3].m128_f32[0], _cameraRotationMatrix.r[3].m128_f32[1], _cameraRotationMatrix.r[3].m128_f32[2], _cameraRotationMatrix.r[3].m128_f32[3]
	};
	return net::Matrix4x4ImGui(matrixData);
}

NetCompatibleImGuiState::NetCompatibleImGuiState(const net::ImGuiState* other) :
	_newData(false),
	_resetLocal(other->requesting_reset()),
	_dirty(true),
	_targetRenderFrequency(other->target_render_frequency()),
	_targetSimulationFrequency(other->target_simulation_frequency()),
	_targetNetworkFrequency(other->target_network_frequency()),
	_currentScenarioIndex(other->current_scenario_index()),
	_paused(other->paused()),
	_gravityOn(other->gravity_on()),
	_timestep(other->timestep()),
	_backgroundColor(0,0,0,0),
	_integrationType(static_cast<helpers::IntegrationType>(other->integration_type())),
	_cameraPositionOffset(XMVectorZero()),
	_cameraRotationMatrix(XMMatrixIdentity())
	{
	for (auto i = 0; i < 4; i++) {
		_backgroundColor[i] = other->background_color()->Get(i);
	}
	_cameraPositionOffset = XMVectorSet(other->camera_position_offset()->Get(0), other->camera_position_offset()->Get(1), other->camera_position_offset()->Get(2), other->camera_position_offset()->Get(3));
	_cameraRotationMatrix = XMMATRIX(
		other->camera_rotation_offset()->m()->Get(0), other->camera_rotation_offset()->m()->Get(1), other->camera_rotation_offset()->m()->Get(2), other->camera_rotation_offset()->m()->Get(3),
		other->camera_rotation_offset()->m()->Get(4), other->camera_rotation_offset()->m()->Get(5), other->camera_rotation_offset()->m()->Get(6), other->camera_rotation_offset()->m()->Get(7),
		other->camera_rotation_offset()->m()->Get(8), other->camera_rotation_offset()->m()->Get(9), other->camera_rotation_offset()->m()->Get(10), other->camera_rotation_offset()->m()->Get(11),
		other->camera_rotation_offset()->m()->Get(12), other->camera_rotation_offset()->m()->Get(13), other->camera_rotation_offset()->m()->Get(14), other->camera_rotation_offset()->m()->Get(15)
	);
}
#endif