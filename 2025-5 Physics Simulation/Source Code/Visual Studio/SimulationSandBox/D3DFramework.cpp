#include "pch.h"
#include "D3DFramework.h"
#include "Resource.h"
#include "ImGuiHandler.h"
#include "NetCompatibleGameObject.h"
#include "SharedMemory.h"
#include "AdvancedSetup.h"
#include "ScenarioVector.h"

using namespace DirectX;
using namespace helpers;
using std::mutex;
using std::lock_guard;

// Copy from shared memory into local cache
void D3DFramework::getInputs() {
	static auto memory = SharedMemory::getInstance();

	lock_guard<mutex> lock(memory->_mx);
	_renderReset = memory->_renderReset;
	_resetPeers = false;
	_physicsReset = false;
	_networkReset = false;

	_actualSimulationSpeed = memory->_actualSimulationSpeed;
	_actualNetworkSpeed = memory->_actualNetworkSpeed;
	_lightPosition = memory->_lightPosition;
	_cameraPosition = memory->_cameraPosition;
	memory->getBackgroundColor(_defaultColor);
	_objectsToDraw.clear();
	for (auto& structVector : memory->_globalObjects) {
		for (auto& objStruct : structVector)
			_objectsToDraw.push_back(objStruct._object);
	}
	_ImGuiState = memory->_ImGuiState;
	memory->_ImGuiState._dirty = false;
	_ImGuiState._dirty = false;
}

//--------------------------------------------------------------------------------------
// Render loop
//--------------------------------------------------------------------------------------
void D3DFramework::render() {
	static auto ImGui = ImGuiHandler::getInstance();

	QueryPerformanceCounter(&_frameStart);
	// ImGui start of loop
	ImGui->renderStart();

	getInputs();
	if (_renderReset) {
		ImGui->setBackgroundColor(_defaultColor);
		ImGui->resetCamera();
		_renderResetThisLoop = true;
	}
	ImGui->calculateActualSimulationFrequency(_actualSimulationSpeed);	
	ImGui->calculateActualNetworkFrequency(_actualNetworkSpeed);
		
	// Calculate camera and light position (could be variable later on)
	const XMVECTOR Eye = XMVector3Transform(_cameraPosition, ImGui->getCameraOrientation());
	const XMVECTOR At = XMVectorSet(0, 0, 0.0f, 0.0f);
	const XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_View = XMMatrixLookAtLH(Eye, At, Up);
	const XMVECTOR& lightPos = _lightPosition;

	// Clear the back buffer
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ImGui->getBackgroundColor());

	// Set depth testing
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView_default, D3D11_CLEAR_DEPTH, 1.0f, 0);
	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView.p, _pDepthStencilView_default);

	// Render objects
	for (const auto& obj : _objectsToDraw) {
		drawSingle(lightPos, Eye, obj);
	}
	{
		// ImGui render and end of loop. Returns true iff scene is reset or switched
		auto currentlyRenderedObjects = calculateRenderedObjects();
		_resetPeers |= ImGui->render(currentlyRenderedObjects, _ImGuiState);

		_physicsReset |= _resetPeers;
		_networkReset |= _resetPeers;
	}

	// Present our back buffer to our front buffer
	_swapChain->Present(0, 0);

	endOfFrame();
	sleepUntilNextFrame();
}

using namespace shapeHelpers;
RenderedObjectStruct D3DFramework::calculateRenderedObjects() const {
	RenderedObjectStruct temp;
	for (auto& obj : _objectsToDraw) {
		switch (obj.getType()) {
		case sphere:
			temp._sphere++;
			break;
		case capsule:
			temp._capsule++;
			break;
		case hollowCube:
			temp._hollowCube++;
			break;
		default:
			throw NotImplementedException();
			break;
		}
	}
	return temp;
}

// Renders a single object
void D3DFramework::drawSingle(const XMVECTOR& lightPos, const XMVECTOR& Eye, const NetCompatibleGameObject& obj) {
	_World = obj.getOrientation();

	const PtrPreset ptrs = _ptrPresets[obj.getPtrType()];
	_pImmediateContext->OMSetDepthStencilState(ptrs._depthStencilState, 1);
	_pImmediateContext->RSSetState(ptrs._rasterizer);
	_pImmediateContext->VSSetShader(ptrs._vertexShader, nullptr, 0);
	_pImmediateContext->PSSetShader(ptrs._pixelShader, nullptr, 0);
	_pImmediateContext->IASetInputLayout(ptrs._inputLayout);

	//update CB
	XMVECTOR baseColor;
	switch (obj.getColor()) {
	case red:
		baseColor = XMVectorSet(0.5f, 0, 0, 1);
		break;
	case yellow:
		baseColor = XMVectorSet(0.3f, 0.3f, 0, 1);
		break;
	case green:
		baseColor = XMVectorSet(0, 0.5f, 0, 1);
		break;
	case blue:
		baseColor = XMVectorSet(0, 0, 0.5f, 1);
		break;
	case undef:
	default:
		baseColor = XMVectorSet(0.0f, 0.0f, 0.0f, 1);
		break;
	}
	const XMMATRIX normalMatrix = XMMatrixInverse(nullptr, _World);

	const ConstantBuffer cb{ XMMatrixTranspose(_World), XMMatrixTranspose(_View), XMMatrixTranspose(_Projection), normalMatrix,
		lightPos, Eye, baseColor, 0 };
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer.p);

	//draw
	ID3D11Buffer* tempVertexBuffer = obj.getVertexBuffer();
#ifdef DEBUGVERTEXBUFFER
		DebugVertexBuffer(tempVertexBuffer, _pImmediateContext);
#endif

	const auto stride = static_cast<UINT>(sizeof(SimpleVertex));
	const auto offset = static_cast<UINT>(0);
	CComPtr<ID3D11Buffer> tempIndexBuffer = obj.getIndexBuffer();
	_pImmediateContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride, &offset);
	_pImmediateContext->IASetIndexBuffer(tempIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	_pImmediateContext->DrawIndexed(obj.getIndexCount(), 0, 0);
}

// Writes from local cache to shared memory
void D3DFramework::endOfFrame() {
	static auto memory = SharedMemory::getInstance();
	static auto ImGui = ImGuiHandler::getInstance();
	lock_guard<mutex> lock(memory->_mx);
	if (memory->_ImGuiState._dirty)
		return;

	memory->_resetPeers |= _resetPeers;
	if (_renderResetThisLoop) {
		_renderResetThisLoop = false;
		memory->_renderReset = false;
	}
	memory->_physicsReset |= _physicsReset;
	memory->_networkReset |= _networkReset;

	// true iff there has been a change. Reset is handled seperately in render
	if (ImGui->updateRenderFrequency(_ImGuiState._targetRenderFrequency) ||
		ImGui->updateSimulationFrequency(_ImGuiState._targetSimulationFrequency) ||
		ImGui->updateNetworkFrequency(_ImGuiState._targetNetworkFrequency) ||
		ImGui->updateScenarioIndex(_ImGuiState._currentScenarioIndex) ||
		ImGui->updatePaused(_ImGuiState._paused) ||
		ImGui->updateGravity(_ImGuiState._gravityOn, _ImGuiState._gravityInverted) ||
		ImGui->updateTimestep(_ImGuiState._timestep) ||
		ImGui->updateBackgroundColor(_ImGuiState._backgroundColor) ||
		ImGui->updateIntegrationType(_ImGuiState._integrationType) ||
		ImGui->updateCameraPosition(_ImGuiState._cameraPositionOffset) ||
		ImGui->updateCameraRotation(_ImGuiState._cameraRotationMatrix)
		) {
#ifndef NOT_DISTRIBUTED
		_ImGuiState._newData = true;
#endif
	}
	memory->_ImGuiState = _ImGuiState;
}

// Enforces target render frequency and calculates actual render frequency
void D3DFramework::sleepUntilNextFrame() {
	static auto ImGui = ImGuiHandler::getInstance();
	const float goalTime = 1.0f / _ImGuiState._targetRenderFrequency;
	QueryPerformanceCounter(&_frameEnd);
	const float time = (static_cast<float>((_frameEnd.QuadPart - _frameStart.QuadPart) * 1.0f / _frequency.QuadPart));

	// Calculate sleep cycles in 15.6 ms chunks
	float netTime = 1000.0f * (goalTime - time);
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
		QueryPerformanceCounter(&_frameEnd);
		newTime = (static_cast<float>((_frameEnd.QuadPart - _frameStart.QuadPart) * 1.0f / _frequency.QuadPart));
		if (newTime >= goalTime)
			break;
	}

	// Calculate final frame time
	ImGui->calculateActualRenderFrequency(newTime);
}



HRESULT D3DFramework::createShaders() {
	std::filesystem::path simulationPath = helpers::getRuntimeFilepath() / "Simulation.fx";
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 2* sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		auto numElements = static_cast<UINT>(ARRAYSIZE(layout));
		// Compile the vertex shader
		CComPtr <ID3DBlob> pVSBlob;
		auto hr = compileShaderFromFile(simulationPath.c_str(), "VS", "vs_5_0", &pVSBlob);
		if (FAILED(hr)) {
			return hr;
		}
		// Create the vertex shader
		hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_ptrPresets[def]._vertexShader);
		if (FAILED(hr)) {
			return hr;
		}
		// Create the input layout
		hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_ptrPresets[def]._inputLayout);
		if (FAILED(hr))
			return hr;
		// Compile the pixel shader
		CComPtr <ID3DBlob> pPSBlob;
		hr = compileShaderFromFile(simulationPath.c_str(), "PS", "ps_5_0", &pPSBlob);
		if (FAILED(hr)) {
			return hr;
		}
		// Create the pixel shader
		hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_ptrPresets[def]._pixelShader);
		if (FAILED(hr)) {
			return hr;
		}
	}
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 2*sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		auto numElements = static_cast<UINT>(ARRAYSIZE(layout));
		// Compile the vertex shader
		CComPtr <ID3DBlob> pVSBlob;
		auto hr = compileShaderFromFile(simulationPath.c_str(), "VS_TEXTURE", "vs_5_0", &pVSBlob);
		if (FAILED(hr)) {
			return hr;
		}
		// Create the vertex shader
		hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_ptrPresets[texture]._vertexShader);
		if (FAILED(hr)) {
			return hr;
		}
		// Create the input layout
		hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_ptrPresets[texture]._inputLayout);
		if (FAILED(hr))
			return hr;
		// Compile the pixel shader
		CComPtr <ID3DBlob> pPSBlob;
		hr = compileShaderFromFile(simulationPath.c_str(), "PS_TEXTURE", "ps_5_0", &pPSBlob);
		if (FAILED(hr)) {
			return hr;
		}
		// Create the pixel shader
		hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_ptrPresets[texture]._pixelShader);
		if (FAILED(hr)) {
			return hr;
		}
	}

	return S_OK;
}
#ifdef _DEBUG
void D3DFramework::DebugVertexBuffer(const CComPtr<ID3D11Buffer>& vertexBuffer, ID3D11DeviceContext* context) {

	if (!vertexBuffer) {
		std::cout << "Vertex buffer is null.\n";
		return;
	}

	// Get the buffer description
	D3D11_BUFFER_DESC bufferDesc;
	vertexBuffer->GetDesc(&bufferDesc);

	// Map the buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = context->Map(vertexBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr)) {
		std::cout << "Failed to map vertex buffer.\n";
		return;
	}

	// Interpret the data as your vertex structure
	auto* vertices = static_cast<SimpleVertex*>(mappedResource.pData);
	size_t vertexCount = bufferDesc.ByteWidth / sizeof(SimpleVertex);

	// Log the vertex data
	for (size_t i = 0; i < vertexCount; ++i) {
		XMFLOAT3 posFloat;
		XMFLOAT3 normFloat;
		XMFLOAT2 texFloat;
		XMStoreFloat3(&posFloat, vertices[i].Pos);
		XMStoreFloat3(&normFloat, vertices[i].Norm);
		XMStoreFloat2(&texFloat, vertices[i].Tex);
		std::cout << "Vertex " << i << " Position (" << posFloat.x << ", " << posFloat.y << ", " << posFloat.z << "), Normal (" <<
			normFloat.x << ", " << normFloat.y << ", " << normFloat.z << "), Tex(" << 
			texFloat.x << ", " << texFloat.y << ")" << std::endl;
	}

	// Unmap the buffer
	context->Unmap(vertexBuffer, 0);
}
#endif
//--------------------------------------------------------------------------------------
// Initialization
//--------------------------------------------------------------------------------------
HRESULT D3DFramework::initWindow(HINSTANCE hInstance, int nCmdShow) {
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>(IDI_SIMULATION));
	wcex.hCursor = reinterpret_cast<HCURSOR>(LoadCursor(nullptr, IDC_ARROW));
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"Starter Template";
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>(IDI_SIMULATION));
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 960, 720 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"Starter Template", L"Direct3D 11 Simulation",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return E_FAIL;

	ShowWindow(_hWnd, nCmdShow);

	return S_OK;
}

HRESULT D3DFramework::compileShaderFromFile(const WCHAR* path, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	auto dwShaderFlags = static_cast<DWORD>(D3DCOMPILE_ENABLE_STRICTNESS);
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	CComPtr <ID3DBlob> pErrorBlob;
	const auto hr = D3DCompileFromFile(path, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr)) {
		if (pErrorBlob)
			OutputDebugStringA(static_cast<const char*>(pErrorBlob->GetBufferPointer()));
		return hr;
	}

	return S_OK;
}

HRESULT D3DFramework::initDevice()
{
	auto hr = static_cast<HRESULT>(S_OK);
	RECT rc;
	GetClientRect(_hWnd, &rc);
	const UINT width = rc.right - rc.left;
	const UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	auto numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	auto numFeatureLevels = static_cast<UINT>(ARRAYSIZE(featureLevels));

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &_pd3dDevice, &_featureLevel, &_pImmediateContext);

		if (hr == static_cast<HRESULT>(E_INVALIDARG))
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &_pd3dDevice, &_featureLevel, &_pImmediateContext);

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	CComPtr <IDXGIFactory1> dxgiFactory;
	{
		CComPtr <IDXGIDevice> dxgiDevice;
		hr = _pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr)) {
			CComPtr <IDXGIAdapter> adapter;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr)) {
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
			}
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	CComPtr <IDXGIFactory2> dxgiFactory2;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));

	// DirectX 11.1 or later
	hr = _pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_pd3dDevice1));
	if (SUCCEEDED(hr)) {
		static_cast<void>(_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_pImmediateContext1)));
	}

	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width = width;
	sd.Height = height;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;

	hr = dxgiFactory2->CreateSwapChainForHwnd(_pd3dDevice, _hWnd, &sd, nullptr, nullptr, &_swapChain1);
	if (SUCCEEDED(hr)) {
		hr = _swapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_swapChain));
	}

	dxgiFactory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);

	if (FAILED(hr))
		return hr;

	// Create a render target view
	CComPtr <ID3D11Texture2D> pBackBuffer;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	if (FAILED(hr))
		return hr;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView.p, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	// Set primitive topology
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
	if (FAILED(hr))
		return hr;

	// Initialize the world matrix
	_World = XMMatrixIdentity();

	// Initialize the projection matrix
	_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / static_cast<FLOAT>(height), 0.01f, 1000000.0f);

	// Depth stencil init
	hr = AdvancedSetup::setupDepthStencil(_pd3dDevice, _pDepthStencil_default, _pDepthStencilView_default, _ptrPresets[def]._depthStencilState, width, height);
	if (FAILED(hr))
		return hr;
	_ptrPresets[texture]._depthStencilState = _ptrPresets[def]._depthStencilState;
	_pImmediateContext->OMSetDepthStencilState(_ptrPresets[def]._depthStencilState, 1);
	// Set rasterizer
	hr = AdvancedSetup::setupRasterizer(_pd3dDevice, _ptrPresets[def]._rasterizer);
	if (FAILED(hr))
		return hr;
	_ptrPresets[texture]._rasterizer = _ptrPresets[def]._rasterizer;
	_pImmediateContext->RSSetState(_ptrPresets[def]._rasterizer);

	hr = AdvancedSetup::setupTextues(_pd3dDevice, _pImmediateContext);
	if (FAILED(hr))
		return hr;

	hr = createShaders();
	if (FAILED(hr))
		return hr;

	threadingInit();
	auto imGui = ImGuiHandler::getInstance();
	std::vector<const char*> scenarioNames = ScenarioVector::getScenarioNames();
	imGui->init(_pd3dDevice, _pImmediateContext, _hWnd, scenarioNames);

	QueryPerformanceFrequency(&_frequency);
	return S_OK;
}

// Once the general DX11 initialization is done, the render thread prepares some data for the physics thread and continues.
void D3DFramework::threadingInit() const {
	auto memory = SharedMemory::getInstance();

	lock_guard<mutex> lock(memory->_mx);
	memory->_simInit._device = _pd3dDevice;
	// After this, simulation thread(s) will stop spinning and initialize, then start
	memory->_simInit._renderDone.store(true);
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK D3DFramework::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static auto app = D3DFramework::getInstance();

	//imgui WndProc
	static auto ImGui = ImGuiHandler::getInstance();
	if (ImGui->wndProc(hWnd, message, wParam, lParam))
		return true;

	std::string msg;
	switch (message) {
		PAINTSTRUCT ps;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case 'A':
			msg = "A pressed";
			break;
		case 'B':
			if (GetKeyState(VK_CONTROL) < 0) {
				msg = "CTRL B pressed";
			}
			else {
				msg = "B pressed";
			}
			break;
		case VK_LEFT:
			msg = "Left cursor pressed";
			break;
		case VK_F1:
			msg = "F1 pressed";
			break;
		default:
			break;
		}
		break;

	case WM_KEYUP:
		switch (wParam) {
		case 'A':
			msg = "A released";
			break;
		case '1':
			msg = "1 released";
			break;
		default:
			break;
		}
		//?
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
D3DFramework::~D3DFramework() {
	std::cout << "~D3DFramework called" << std::endl;
	try {
		if (_pImmediateContext)
			_pImmediateContext->ClearState();
	}
	catch (...) {

	}
}