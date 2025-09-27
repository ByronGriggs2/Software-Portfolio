#include "pch.h"
#include "ImGuiHandler.h"
#include "NetCompatibleImGuiState.h"

using namespace DirectX;
using namespace helpers;

ImGuiHandler::ImGuiHandler() {
	_integrationType = euler;
}

ImGuiHandler::~ImGuiHandler() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiHandler::renderStart() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

bool ImGuiHandler::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	return false;
}

void ImGuiHandler::init(CComPtr<ID3D11Device> device, CComPtr<ID3D11DeviceContext> context, HWND hWnd, const std::vector<const char*>& scenarioList) {
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
	_scenarioNames = scenarioList;
}

bool ImGuiHandler::render(const RenderedObjectStruct& currentObjects, const NetCompatibleImGuiState& inputs) {
	if (_firstFrame || _secondFrame) {
		bool window = true;
		const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
		_integrationDropdownSize = findIntegrationDropdownSize(window, windowFlags);
		_startStopResetSize = findStartStopResetSize(window, windowFlags);
		_gravityButtonsSize = findGravityButtonsSize(window, windowFlags);
		int tempIndex = 0;
		_scenarioDropdownSize = findScenarioDropdownSize(tempIndex, window, windowFlags);

		_targetRenderFrequency = inputs._targetRenderFrequency;
		_targetSimulationFrequency = inputs._targetSimulationFrequency;
		_targetNetworkFrequency = inputs._targetNetworkFrequency;
		_currentScenarioIndex = inputs._currentScenarioIndex;
		_isSimulating = !inputs._paused;
		_gravityOn = inputs._gravityOn;
		_gravityInverted = inputs._gravityInverted;
		_timestep = inputs._timestep;
		for (auto i = 0; i < 4; i++) {
			_currentBackground[i] = inputs._backgroundColor[i];
		}
		_integrationType = inputs._integrationType;
		_cameraPositionOffset = inputs._cameraPositionOffset;
		_cameraRotationMatrix = inputs._cameraRotationMatrix;

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (_firstFrame) {
			_firstFrame = false;
			_secondFrame = true;
		}
		else
			_secondFrame = false;
		return false;
	}


	bool reset = guiFrame(currentObjects);
	//imgui end of loop
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return reset;
}

//Gui drawing on each frame
bool ImGuiHandler::guiFrame(const RenderedObjectStruct& currentObjects) {
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
	bool window = true;
	const ImVec2 screenSize = ImGui::GetIO().DisplaySize;

	// Right side
	float startHeight = 0.0f;
	if (drawStartStopReset(ImVec2(screenSize.x-_startStopResetSize.x, startHeight), window, windowFlags))
		return true;
	startHeight += 34.0f;

	drawGravityButtons(ImVec2(screenSize.x - _gravityButtonsSize.x, startHeight), window, windowFlags);
	startHeight += 34.0f;

	if (drawScenarioDropdown(ImVec2(screenSize.x - _scenarioDropdownSize.x, startHeight), window, windowFlags))
		return true;
	startHeight += 34.0f;

	drawIntegrationDropdown(ImVec2(screenSize.x - _integrationDropdownSize.x, startHeight), window, windowFlags);

	// Left side
	startHeight = 0.0f;
	drawColorPicker(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 34.0f;
	drawTimestepSlider(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 34.0f;
	drawSimFrequencySlider(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 34.0f;
	drawRenderFrequencySlider(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 34.0f;
	drawNetworkFrequencySlider(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 34.0f;
	drawData(startHeight, currentObjects, window, windowFlags);

	cameraControls();

	return false;
}

ImVec2 ImGuiHandler::findStartStopResetSize(bool& window, const ImGuiWindowFlags& windowFlags) {
	const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
	const ImVec2 windowPos = ImVec2(screenSize.x + 10.0f, 0);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);

	ImGui::Begin("Start/Stop buttons test", &window, windowFlags);
	ImGui::Button("Pause sim");
	ImGui::SameLine();
	ImGui::Button("Reset sim");
	ImVec2 tempSize = ImGui::GetWindowSize();
	ImGui::End();
	return tempSize;
}

bool ImGuiHandler::drawStartStopReset(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Start/Stop buttons", &window, windowFlags);
	// start/stop animation
	if (_isSimulating) {
		_isSimulating = !ImGui::Button("Pause sim");
	}
	else {
		_isSimulating = ImGui::Button("Start sim");
	}
	//reset animation
	ImGui::SameLine();
	bool temp = ImGui::Button("Reset sim");
	ImGui::End();
	return temp;
}

ImVec2 ImGuiHandler::findGravityButtonsSize(bool& window, const ImGuiWindowFlags& windowFlags) {
	const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
	const ImVec2 windowPos = ImVec2(screenSize.x + 10.0f, 0);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
	ImGui::Begin("Gravity buttons test", &window, windowFlags);
	ImGui::Button("Toggle Gravity");
	ImGui::SameLine();
	ImGui::Button("Invert Gravity");
	ImVec2 tempSize = ImGui::GetWindowSize();
	ImGui::End();
	return tempSize;
}

void ImGuiHandler::drawGravityButtons(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Gravity buttons", &window, windowFlags);
	if (ImGui::Button("Toggle Gravity"))
		_gravityOn = !_gravityOn;
	ImGui::SameLine();
	if (ImGui::Button("Invert Gravity"))
		_gravityInverted = !_gravityInverted;
	ImGui::End();
}

ImVec2 ImGuiHandler::findScenarioDropdownSize(int& currentScenarioIndex, bool& window, const ImGuiWindowFlags& windowFlags) {
	const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
	const ImVec2 windowPos = ImVec2(screenSize.x + 10.0f, 0);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);

	static int newScenarioIndex = 0;

	ImGui::Begin("Scene select dropdown test", &window, windowFlags);
	ImGui::Combo("##Select Scenario", &newScenarioIndex, _scenarioNames.data(), static_cast<int>(_scenarioNames.size()));
	ImVec2 tempSize = ImGui::GetWindowSize();
	ImGui::End();
	return tempSize;
}

bool ImGuiHandler::drawScenarioDropdown(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Scene select dropdown", &window, windowFlags);
	static int newScenarioIndex = 0;

	if (ImGui::Combo("##Select Scenario", &newScenarioIndex, _scenarioNames.data(), static_cast<int>(_scenarioNames.size()))) {
		if (_currentScenarioIndex != newScenarioIndex) {
			_currentScenarioIndex = newScenarioIndex;
			ImGui::End();
			return true;
		}
	}
	ImGui::End();
	return false;
}

ImVec2 ImGuiHandler::findIntegrationDropdownSize(bool& window, const ImGuiWindowFlags& windowFlags) {
	const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
	const ImVec2 windowPos = ImVec2(screenSize.x + 10.0f, 0);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);

	ImGui::Begin("Integration type test", &window, windowFlags);
	ImGui::Combo("##Integration Type", reinterpret_cast<int*>(&_integrationType), "Euler\0Semi-Implicit Euler\0Runge-Kutta\0");
	ImVec2 tempSize = ImGui::GetWindowSize();
	ImGui::End();
	return tempSize;
}

void ImGuiHandler::drawIntegrationDropdown(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Integration type dropdown", &window, windowFlags);
	ImGui::Combo("##Integration Type", reinterpret_cast<int*>(&_integrationType), "Euler\0Semi-Implicit Euler\0Runge-Kutta\0");
	ImGui::End();
}

void ImGuiHandler::drawColorPicker(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("ColorPicker", &window, windowFlags);
	ImGui::ColorEdit3("Background Color", _currentBackground.get(), ImGuiColorEditFlags_NoInputs);
	ImGui::End();
}

void ImGuiHandler::drawTimestepSlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Timestep slider", &window, windowFlags);
	ImGui::SliderFloat("##timestep", &_timestep, 0.005f, 0.05f, "Timestep: %.3f");
	_timestep = roundf(_timestep / 0.005f) * 0.005f;
	ImGui::End();
}

void ImGuiHandler::drawSimFrequencySlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Target simulation frequency slider", &window, windowFlags);
	static int targetFrequency = 200;
	ImGui::SliderInt("##Render frequency", &_targetSimulationFrequency, 1, 1000, "Simulation Frequency: %d");
	ImGui::End();
}

void ImGuiHandler::drawRenderFrequencySlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Target render frequency slider", &window, windowFlags);
	ImGui::SliderInt("##Render frequency", &_targetRenderFrequency, 1, 120, "Render Frequency: %d");
	ImGui::End();
}

void ImGuiHandler::drawNetworkFrequencySlider(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Target network frequency slider", &window, windowFlags);
	ImGui::SliderInt("##Network frequency", &_targetNetworkFrequency, 1, 60, "Network Frequency: %d");
	ImGui::End();
}

void ImGuiHandler::drawData(float startHeight, const helpers::RenderedObjectStruct& objects, bool& window, const ImGuiWindowFlags& windowFlags) {
	startHeight += drawCurrentObjects(ImVec2(0, startHeight), objects, window, windowFlags);
	drawSimulationFrequency(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 28.0f;
	drawRenderFrequency(ImVec2(0, startHeight), window, windowFlags);
	startHeight += 28.0f;
	drawNetworkFrequency(ImVec2(0, startHeight), window, windowFlags);
}

float ImGuiHandler::drawCurrentObjects(const ImVec2& position, const helpers::RenderedObjectStruct& objects, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("object count label", &window, windowFlags);
	ImVec2 startCursor = ImGui::GetCursorScreenPos();
	ImGui::Text("Number of objects\n Spheres: %d\n Capsules: %d\n Hollow Cubes: %d", objects._sphere, objects._capsule, objects._hollowCube);
	ImVec2 endCursor = ImGui::GetCursorScreenPos();
	float contentHeight = endCursor.y - startCursor.y + 11;
	ImGui::End();
	return contentHeight;
}

void ImGuiHandler::drawSimulationFrequency(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Actual simulation frequency label", &window, windowFlags);
	ImGui::Text("Actual simulation frequency: %.3f", _actualSimulationFrequency);
	ImGui::End();
}

void ImGuiHandler::drawRenderFrequency(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Actual render frequency label", &window, windowFlags);
	ImGui::Text("Actual render frequency: %.3f", _actualRenderFrequency);
	ImGui::End();
}

void ImGuiHandler::drawNetworkFrequency(const ImVec2& position, bool& window, const ImGuiWindowFlags& windowFlags) {
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);
	ImGui::Begin("Actual network frequency label", &window, windowFlags);
	ImGui::Text("Actual network frequency: %.3f", _actualNetworkFrequency);
	ImGui::End();
}

void ImGuiHandler::cameraControls() {
	// Camera controls
	const float FRAMERATE = 60.0f;
	if (ImGui::IsKeyDown(ImGuiKey_W)) {
		_cameraPositionOffset += XMVectorSet(0, -1.0f / FRAMERATE, 1.0f / FRAMERATE, 0);
	}
	if (ImGui::IsKeyDown(ImGuiKey_A)) {
		_cameraRotationMatrix *= XMMatrixRotationRollPitchYaw(0, XM_PIDIV2 / FRAMERATE, 0);
	}
	if (ImGui::IsKeyDown(ImGuiKey_S)) {
		_cameraPositionOffset += XMVectorSet(0, 1.0f / FRAMERATE, -1.0f / FRAMERATE, 0);
	}
	if (ImGui::IsKeyDown(ImGuiKey_D)) {
		_cameraRotationMatrix *= XMMatrixRotationRollPitchYaw(0, -XM_PIDIV2 / FRAMERATE, 0);
	}
}

void ImGuiHandler::resetCamera() { 
	_cameraPositionOffset = XMVectorZero();
	_cameraRotationMatrix = XMMatrixIdentity();
}
void ImGuiHandler::updateActualSimFrequency(float framerate) { 
	_actualSimulationFrequency = framerate; 
}
void ImGuiHandler::updateActualRenderFrequency(float framerate) { 
	_actualRenderFrequency = framerate; 
}
void ImGuiHandler::updateActualNetworkFrequency(float framerate) {
	_actualNetworkFrequency = framerate;
}

void ImGuiHandler::setBackgroundColor(std::shared_ptr<float[]> colorToUse) {
	for (auto i = 0; i < 4; i++) {
		_currentBackground[i] = colorToUse.get()[i];
	}
}

void ImGuiHandler::calculateActualSimulationFrequency(float simSpeedThisLoop) {
	static int i = 0;
	static float framerateSum = 0;

	framerateSum += simSpeedThisLoop;
	if (i == 9) {
		updateActualSimFrequency(framerateSum / 10.0f);
		i = 0;
		framerateSum = 0;
	}
	else {
		i++;
	}
}

void ImGuiHandler::calculateActualRenderFrequency(float renderSpeedThisLoop) {
	static int i = 0;
	static float framerateSum = 0;

	framerateSum += renderSpeedThisLoop;
	if (i == 9) {
		updateActualRenderFrequency(1.0f / (framerateSum / 10.0f));
		i = 0;
		framerateSum = 0;
	}
	else {
		i++;
	}
}

void ImGuiHandler::calculateActualNetworkFrequency(float networkSpeedThisLoop) {
	static int i = 0;
	static float framerateSum = 0;

	framerateSum += networkSpeedThisLoop;
	if (i == 9) {
		updateActualNetworkFrequency(framerateSum / 10.0f);
		i = 0;
		framerateSum = 0;
	}
	else {
		i++;
	}
}

void ImGuiHandler::setValuesFromNetwork(const NetCompatibleImGuiState& state) {
	_targetRenderFrequency = state._targetRenderFrequency;
	_targetSimulationFrequency = state._targetSimulationFrequency;
	_targetNetworkFrequency = state._targetNetworkFrequency;
	_currentScenarioIndex = state._currentScenarioIndex;
	_isSimulating = !state._paused;
	_gravityOn = state._gravityOn;
	_gravityInverted = state._gravityInverted;
	_timestep = state._timestep;
	_integrationType = state._integrationType;
	_cameraPositionOffset = state._cameraPositionOffset;
	_cameraRotationMatrix = state._cameraRotationMatrix;
}