#pragma once

#include <memory>
#include <vector>
#include "imgui.h"
#include "DirectXMath.h"
#include "shapeHelpers.h"

namespace helpers {
	struct RenderedObjectStruct;
	enum IntegrationType;
}

struct NetCompatibleImGuiState;

/////////////////////
// Singleton

class ImGuiHandler final
{
	std::vector<const char*> _scenarioNames;
	int _targetRenderFrequency = 60;
	float _actualRenderFrequency = 60.0f;
	int _targetSimulationFrequency = 200;
	float _actualSimulationFrequency = 200.0f;
	int _targetNetworkFrequency = 20;
	float _actualNetworkFrequency = 20.0f;
	int _currentScenarioIndex = 0;
	bool _isSimulating = true;
	bool _gravityOn = true;
	bool _gravityInverted = false;
	float _timestep = 0.005f;
	helpers::IntegrationType _integrationType;
	
	std::unique_ptr<float[]> _currentBackground = std::unique_ptr<float[]>(new float[4] { 0.0f, 0.0f, 0.0f, 0.0f });
	DirectX::XMVECTOR _cameraPositionOffset = DirectX::XMVectorZero();
	DirectX::XMMATRIX _cameraRotationMatrix = DirectX::XMMatrixIdentity();

	bool _firstFrame = true;
	bool _secondFrame = false;

	bool guiFrame(const helpers::RenderedObjectStruct& currentObjects);

	// Todo when polishing: remove unnecessary parameters

	// Right side
	ImVec2 _startStopResetSize = ImVec2(0, 0);
	ImVec2 findStartStopResetSize(bool& window, const ImGuiWindowFlags& flags);
	bool drawStartStopReset(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);

	ImVec2 _gravityButtonsSize = ImVec2(0, 0);
	ImVec2 findGravityButtonsSize(bool& window, const ImGuiWindowFlags& flags);
	void drawGravityButtons(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);
	
	ImVec2 _scenarioDropdownSize = ImVec2(0, 0);
	ImVec2 findScenarioDropdownSize(int& currentScenarioIndex, bool& window, const ImGuiWindowFlags& windowFlags);
	bool drawScenarioDropdown(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags);

	ImVec2 _integrationDropdownSize = ImVec2(0, 0);
	ImVec2 findIntegrationDropdownSize(bool& window, const ImGuiWindowFlags& windowFlags);
	void drawIntegrationDropdown(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);

	// Left side
	void drawColorPicker(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);
	void drawTimestepSlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);
	void drawSimFrequencySlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);
	void drawRenderFrequencySlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);
	void drawNetworkFrequencySlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& flags);
	
	void drawData(float startHeight, const helpers::RenderedObjectStruct& currentObjects, bool& window, const ImGuiWindowFlags& windowFlags);
	float drawCurrentObjects(const ImVec2& position, const helpers::RenderedObjectStruct& currentObjects, bool& window, const ImGuiWindowFlags& windowFlags);
	void drawSimulationFrequency(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags);
	void drawRenderFrequency(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags);
	void drawNetworkFrequency(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags);

	// Not drawn
	void cameraControls();
	void updateActualSimFrequency(float framerate);
	void updateActualRenderFrequency(float framerate);
	void updateActualNetworkFrequency(float framerate);

	ImGuiHandler();
public:
	ImGuiHandler(const ImGuiHandler& other) = delete;
	ImGuiHandler& operator=(const ImGuiHandler& other) = delete;
	ImGuiHandler(const ImGuiHandler&& other) = delete;
	ImGuiHandler& operator=(const ImGuiHandler&&) = delete;

	static std::shared_ptr<ImGuiHandler> getInstance() { 
		static std::shared_ptr<ImGuiHandler> instance(new ImGuiHandler);
		return instance;
	}

	// Not sure if this needs to run before ~D3DFramework. Currently assuming it does not
	~ImGuiHandler();

	bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void init(CComPtr<ID3D11Device> device, CComPtr<ID3D11DeviceContext> context, HWND hWnd, const std::vector<const char*>& sceneList);

	void renderStart();

	// Returns true iff the scene is reset, including if scene is switched
	bool render(const helpers::RenderedObjectStruct& currentObjects, const NetCompatibleImGuiState& inputs);

	float* getBackgroundColor() const { return _currentBackground.get(); }
	DirectX::XMMATRIX getCameraOrientation() const { return DirectX::XMMatrixTranslationFromVector(_cameraPositionOffset) * _cameraRotationMatrix; }
	bool updateRenderFrequency(int& freq) const {
		if (freq == _targetRenderFrequency)
			return false;
		freq = _targetRenderFrequency;
		return true;
	}
	bool updateSimulationFrequency(int& freq) const {
		if (freq == _targetSimulationFrequency)
			return false;
		freq = _targetSimulationFrequency;
		return true;
	}
	bool updateNetworkFrequency(int& freq) const {
		if (freq == _targetNetworkFrequency)
			return false;
		freq = _targetNetworkFrequency;
		return true;
	}
	bool updateScenarioIndex(int& index) const {
		if (index == _currentScenarioIndex)
			return false;
		index = _currentScenarioIndex;
		return true;
	}
	bool updatePaused(bool& paused) const {
		if (paused == !_isSimulating)
			return false;
		paused = !_isSimulating;
		return true;
	}
	bool updateGravity(bool& on, bool& inverse) const {
		if (on == _gravityOn && inverse == _gravityInverted)
			return false;
		on = _gravityOn;
		inverse = _gravityInverted;
		return true;
	}
	bool updateTimestep(float& timestep) const {
		if (abs(timestep - _timestep) < EPSILON)
			return false;
		timestep = _timestep;
		return true;
	}
	bool updateBackgroundColor(float* color) const {
		bool changed = false;
		for (auto i = 0; i < 4; i++) {
			if (abs(color[i] - _currentBackground[i]) >= EPSILON) {
				color[i] = _currentBackground[i];
				changed = true;
			}
		}
		return changed;
	}
	bool updateIntegrationType(helpers::IntegrationType& integ) const {
		if (integ == _integrationType)
			return false;
		integ = _integrationType;
		return true;
	}
	bool updateCameraPosition(DirectX::XMVECTOR& pos) const {
		if (shapeHelpers::vec3Equal(pos, _cameraPositionOffset))
			return false;
		pos = _cameraPositionOffset;
		return true;
	}
	bool updateCameraRotation(DirectX::XMMATRIX& rot) const {
		for (int i = 0; i < 4; i++) {
			auto& tempRot = rot.r[i];
			auto& tempMine = _cameraRotationMatrix.r[i];
			if (!shapeHelpers::vec3Equal(tempRot, tempMine)) {
				rot = _cameraRotationMatrix;
				return true;
			}
		}
		return false;
	}
	void calculateActualSimulationFrequency(float simSpeedThisLoop);
	void calculateActualNetworkFrequency(float networkSpeedThisLoop);
	void calculateActualRenderFrequency(float renderSpeedThisLoop);

	void setValuesFromNetwork(const NetCompatibleImGuiState& state);

	void resetCamera();
	void setBackgroundColor(std::shared_ptr<float[]> colorToUse);
};

