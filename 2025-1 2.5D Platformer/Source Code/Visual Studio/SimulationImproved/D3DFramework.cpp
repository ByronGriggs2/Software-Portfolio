#include "D3DFramework.h"
#include <directxcolors.h>
#include "Resource.h"
#include "GenerateObj.h"
#include "ConfigReader.h"
#include "StringLiterals.h"

std::unique_ptr<D3DFramework> D3DFramework::_instance = std::make_unique<D3DFramework>();

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK D3DFramework::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	auto& app = D3DFramework::getInstance();
	//imgui WndProc
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
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
		case 'R':
			app._rotation = 0.0f;
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

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT D3DFramework::initWindow(HINSTANCE hInstance, int nCmdShow) {
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


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT D3DFramework::compileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
	const auto hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr)) {
		if (pErrorBlob)
			OutputDebugStringA(static_cast<const char*>(pErrorBlob->GetBufferPointer()));
		return hr;
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT D3DFramework::initDevice()
{
	//complete.txt exists as sort of a checksum to ensure the .obj files are up to date
	{
		std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / MISC;
		if (!std::filesystem::exists(tempDir)) {
			GenerateObj& objCreator = GenerateObj::getInstance();
			objCreator.generate();
		}
		else {
			std::string filename = (tempDir / "complete.txt").string();
			const std::ifstream ifs(filename);
			if (!ifs.is_open()) {
				GenerateObj& objCreator = GenerateObj::getInstance();
				objCreator.generate();
			}
		}
	}
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

	hr = createTextures();
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

	//default depth stencil
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = _pd3dDevice->CreateTexture2D(&descDepth, nullptr, &_pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(_pDepthStencil, &descDSV, &_pDepthStencilView);

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));
	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = _pd3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &_pDepthStencilState);

	//fire depth stencil
	depthStencilStateDesc.DepthEnable = FALSE;
	depthStencilStateDesc.StencilEnable = FALSE;
	_pd3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &_pDepthStencilState_particle);
	//skybox depth stencil
	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	_pd3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &_pDepthStencilState_sky);

	//blend
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable = FALSE;
	BlendState.IndependentBlendEnable = FALSE;
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _pd3dDevice->CreateBlendState(&BlendState, &_pBlendState);

	BlendState.RenderTarget[0].BlendEnable = FALSE;
	_pd3dDevice->CreateBlendState(&BlendState, &_pDefaultBlend);

	float blendFactor[4] = { 0,0,0,0 };
	const UINT sampleMask = 0xffffffff;
	_pImmediateContext->OMSetBlendState(_pBlendState, blendFactor, sampleMask);

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

	hr = createShaders();
	if (FAILED(hr))
		return hr;

	//Use ConfigReader, which parses config.txt then calls ReadObj::read() and ReadObj::generate(...), which actually reads in .obj files
	auto& fileReader = ConfigReader::getInstance();
	_player = fileReader.read(_objectsToDraw);
	//create vertex/index buffers. This is the only time the vertex/index buffers are changed
	bufferAll();

	//create solid ground map
	for (auto& object : _objectsToDraw) {
		if (object.getType() == "platform") {
			hitBox currentBox;
			for (const auto& vertex : object.getVertices()) {
				if (vertex.Pos.x > currentBox.p1.x) {
					currentBox.p1.x = vertex.Pos.x;
				}
				if (vertex.Pos.x < currentBox.p2.x) {
					currentBox.p2.x = vertex.Pos.x;
				}
				if (vertex.Pos.z > currentBox.p1.z) {
					currentBox.p1.z = vertex.Pos.z;
				}
				if (vertex.Pos.z < currentBox.p2.z) {
					currentBox.p2.z = vertex.Pos.z;
				}
			}
			_solidGround.push_back(currentBox);
		}
	}

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

	//imgui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX11_Init(_pd3dDevice, _pImmediateContext);

	//rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	//default rasterizer
	hr = _pd3dDevice->CreateRasterizerState(&rasterDesc, &_pRast_default);
	_pImmediateContext->RSSetState(_pRast_default);

	//fire and skybox rasterizer
	rasterDesc.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&rasterDesc, &_pRast_particle);
	hr = _pd3dDevice->CreateRasterizerState(&rasterDesc, &_pRast_sky);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
D3DFramework::~D3DFramework() {
	try {
		if (_pImmediateContext)
			_pImmediateContext->ClearState();
		//imgui shutdown
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	catch (...) {

	}
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
const int TARGET_FRAMERATE = 1000;
const std::chrono::milliseconds TARGET_FRAME_DURATION(1000 / TARGET_FRAMERATE);
void D3DFramework::render() {
	auto frameStartTime = std::chrono::high_resolution_clock::now();
	//imgui start of loop
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//user inputs
	controls();
	physics();

	//camera tracks character
	const XMFLOAT3 rawPos = _player.getPos();
	const XMVECTOR Eye = XMVectorSet(rawPos.x, rawPos.y + 0.0f, rawPos.z + _currentDistance, 1.0f);
	const XMVECTOR At = XMVectorSet(rawPos.x, rawPos.y + 0.0f, rawPos.z, 0.0f);
	const XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_View = XMMatrixLookAtLH(Eye, At, Up);
	const XMVECTOR lightPos = XMVectorSet(30000, 20000, 100000, 1);

	//time
	static float t = 0.0f;
	if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += static_cast<float>(XM_PI) * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		const ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}

	//Rotate various objects
	_rotation += 0.001f;

	//
	// Clear the back buffer
	//
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, Colors::MidnightBlue);

	//set depth testing
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView.p, _pDepthStencilView);

	//draw objects
	{
		drawSingle(lightPos, Eye, t, _player.getPlayer());
		drawSingle(lightPos, Eye, t, _player.getArrow());
	}
	for (const auto& object : _objectsToDraw) {
		if (!object.isCollected())
		drawSingle(lightPos, Eye, t, object);
	}

	//imGUI stuff
	guiFrame();
	//imgui end of loop
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//
	// Present our back buffer to our front buffer
	//
	_swapChain->Present(0, 0);

	// Measure the end time of the frame
	auto frameEndTime = std::chrono::high_resolution_clock::now();
	const auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime - frameStartTime);

	// Sleep for the remaining time to maintain the target framerate
	if (frameDuration < TARGET_FRAME_DURATION) {
		std::this_thread::sleep_for(TARGET_FRAME_DURATION - frameDuration);
	}
}

void D3DFramework::controls() {
	//exit (ESC)
	if ((GetAsyncKeyState(0x1B) & 0x8000) != 0) {
		PostQuitMessage(0);
	}
	//reset (R)
	else if ((GetAsyncKeyState(0x52) & 0x8000) != 0) {
		_spotlightOn = 0.0f;
		_timeFactor = 1.0f;
		_currentDistance = -80.0f;
		_player.newAction(Action::reset);
		for (auto& obj : _objectsToDraw) {
			obj.unCollect();
		}
		_objectsCollected = 0;
	}
	//daylight/torch (T)
	else if ((GetAsyncKeyState(0x54) & 0x8000) != 0) {
		if (!_spotlightDebounce) {
			_spotlightDebounce = true;
			if (-0.1 < _spotlightOn && 0.1 > _spotlightOn) {
				_spotlightOn = 1.0f;
			}
			else {
				_spotlightOn = 0.0f;
			}
		}
		return;
	}
	//time factor (<)
	else if ((GetAsyncKeyState(0xBC) & 0x8000) != 0) {
		if (!_timeDownDebounce) {
			_timeDownDebounce = true;
			if (_timeFactor > 0.21f) {
				_timeFactor -= 0.2f;
				_player.newAction(Action::decreaseSpeed);
			}
		}
		return;
	}
	//time factor (>)
	else if ((GetAsyncKeyState(0xBE) & 0x8000) != 0) {
		if (!_timeUpDebounce) {
			_timeUpDebounce = true;
			if (_timeFactor < 2.79f) {
				_timeFactor += 0.2f;
				_player.newAction(Action::increaseSpeed);
			}
		}
		return;
	}
	//zoom in
	else if ((GetAsyncKeyState(0xBB) & 0x8000) != 0) {
		if (_currentDistance <= -2.0f) {
			_currentDistance += 1;
		}
	}
	//zoom out
	else if ((GetAsyncKeyState(0xBD) & 0x8000) != 0) {
		_currentDistance -= 1;
	}
	//Spacebar
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		_player.newAction(Action::jump);
	//upLeft
	else if ((GetAsyncKeyState(0x41) & 0x8000) != 0 &&
		     (GetAsyncKeyState(0x57) & 0x8000) != 0) {
		_player.newAction(Action::upLeft);
	}
	//upRight
	else if ((GetAsyncKeyState(0x44) & 0x8000) != 0 &&
		     (GetAsyncKeyState(0x57) & 0x8000) != 0) {
		_player.newAction(Action::upRight);
	}
	//downLeft
	else if ((GetAsyncKeyState(0x41) & 0x8000) != 0 &&
		     (GetAsyncKeyState(0x53) & 0x8000) != 0) {
		_player.newAction(Action::downLeft);
	}
	//downRight
	else if ((GetAsyncKeyState(0x44) & 0x8000) != 0 &&
		     (GetAsyncKeyState(0x53) & 0x8000) != 0) {
		_player.newAction(Action::downRight);
	}
	//Left
	else if ((GetAsyncKeyState(0x41) & 0x8000) != 0) {
		_player.newAction(Action::left);
	}
	//Up
	else if ((GetAsyncKeyState(0x57) & 0x8000) != 0) {
		_player.newAction(Action::up);
	}
	//Right
	else if ((GetAsyncKeyState(0x44) & 0x8000) != 0) {
		_player.newAction(Action::right);
	}
	//Down
	else if ((GetAsyncKeyState(0x53) & 0x8000) != 0) {
		_player.newAction(Action::down);
	}
	else {
		_player.newAction(Action::idle);
	}
	_spotlightDebounce = false;
	_timeDownDebounce = false;
	_timeUpDebounce = false;
}

void D3DFramework::physics() {
	_player.physics(abovePlatform());

	//check for collection
	for (auto& obj : _objectsToDraw) {
		if (obj.isCollectable() && !obj.isCollected()) {
			XMFLOAT3 temp = _player.getPos();
			const XMVECTOR vec1 = XMVectorSet(temp.x, temp.y, temp.z, 1.0f);
			temp = obj.getPos();
			const XMVECTOR vec2 = XMVectorSet(temp.x, temp.y, temp.z, 1.0f);
			const XMVECTOR diff = XMVectorSubtract(vec1, vec2);
			const XMVECTOR length = XMVector3Length(diff);
			XMStoreFloat(&temp.x, length);
			if (temp.x <= 15.0f) {
				obj.collect();
				_objectsCollected++;
			}
		}
	}
}

bool D3DFramework::abovePlatform() {
	bool above = false;
	const XMFLOAT3 playerPos = _player.getPos();
	for (const auto& plat : _solidGround) {
		if (playerPos.x <= plat.p1.x &&
			playerPos.z <= plat.p1.z &&
			playerPos.x >= plat.p2.x &&
			playerPos.z >= plat.p2.z) {
			above = true;
		}
	}
	return above;
}

void D3DFramework::bufferAll() {
	{
		bufferSingle(_player.getPlayer());
		bufferSingle(_player.getArrow());
	}
	for (auto& object : _objectsToDraw) {
		bufferSingle(object);
	}
}

void D3DFramework::bufferSingle(CompleteObject& obj) {
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	obj.setIndexCount();
	bd.ByteWidth = obj.getVertexByteWidth();
	std::vector<SimpleVertex> tempVertices = obj.getVertices();
	InitData.pSysMem = &tempVertices[0];
	CComPtr<ID3D11Buffer> tempVertexBuffer = nullptr;
	_pd3dDevice->CreateBuffer(&bd, &InitData, &tempVertexBuffer);
	obj.setVertexBuffer(tempVertexBuffer);

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = obj.getIndexByteWidth();
	std::vector<WORD> tempIndexVector = obj.getIndices();
	InitData.pSysMem = &tempIndexVector[0];
	CComPtr<ID3D11Buffer> tempIndexBuffer = nullptr;
	_pd3dDevice->CreateBuffer(&bd, &InitData, &tempIndexBuffer);
	obj.setIndexBuffer(tempIndexBuffer);
}

//All of these functions are used to draw a single object 
void D3DFramework::drawSingle(const XMVECTOR& lightPos, const XMVECTOR& Eye, float time, const CompleteObject& obj) {
	const auto stride = static_cast<UINT>(sizeof(SimpleVertex));
	const auto offset = static_cast<UINT>(0);

	const std::string objType = obj.getType();

	drawRotation(obj.getRotation(), obj);
	const int texType = drawTextureType(objType);
	drawPlayerMove(objType);
	drawDepthStencil(objType);
	drawBlend(objType);
	drawRasterizer(objType);
	drawShader(objType);

	//update CB
	const ConstantBuffer cb{ XMMatrixTranspose(_World), XMMatrixTranspose(_View), XMMatrixTranspose(_Projection),
		lightPos, Eye, time * _timeFactor, _spotlightOn, texType };
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer.p);

	//draw
	ID3D11Buffer* tempVertexBuffer = obj.getVertexBuffer();
	CComPtr<ID3D11Buffer> tempIndexBuffer = obj.getIndexBuffer();
	_pImmediateContext->IASetVertexBuffers(0, 1, &tempVertexBuffer, &stride, &offset);
	_pImmediateContext->IASetIndexBuffer(tempIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	_pImmediateContext->DrawIndexed(obj.getIndexCount(), 0, 0);
}
//rotates an object such as a collectable sphere
void D3DFramework::drawRotation(Rotation rot, const CompleteObject& obj) {
	if (rot == Rotation::null) {
		_World = XMMatrixIdentity();
	}
	else if (rot == Rotation::cw) {
		_World = XMMatrixTranslation(-obj.getPos().x, -obj.getPos().y, -obj.getPos().z);
		_World *= XMMatrixRotationY(-_rotation * _timeFactor);
		_World *= XMMatrixTranslation(obj.getPos().x, obj.getPos().y, obj.getPos().z);
	}
	else {
		_World = XMMatrixTranslation(-obj.getPos().x, -obj.getPos().y, -obj.getPos().z);
		_World *= XMMatrixRotationY(_rotation * _timeFactor);
		_World *= XMMatrixTranslation(obj.getPos().x, obj.getPos().y, obj.getPos().z);
	}
}
//sets the texture of the object
int D3DFramework::drawTextureType(const std::string& type) const {
	int texType;
	if (type == "cliff") {
		texType = 0;
	}
	else if (type == "ground") {
		texType = 1;
	}
	else if (type == "platform") {
		texType = 2;
	}
	else if (type == "player") {
		texType = 3;
	}
	else {
		texType = -1;
	}
	return texType;
}
//translate the player and arrow
void D3DFramework::drawPlayerMove(const std::string& type) {
	if (type == "player") {
		XMFLOAT3 playerPos = _player.getPos();
		const XMFLOAT3 playerSpawn = _player.getPlayerSpawn();
		playerPos.x -= playerSpawn.x; playerPos.y -= playerSpawn.y; playerPos.z -= playerSpawn.z;;
		_World *= XMMatrixTranslation(playerPos.x, playerPos.y, playerPos.z);
	}
	else if (type == "arrow") {
		const float PI = 3.1415926535f;
		const float angleInQuarters = (static_cast<float>(Direction::d_up) - static_cast<float>(_player.look()));
		const float angleInRadians = -PI * angleInQuarters / 4;
		//translate to origin and up by radius of rotation
		const XMFLOAT3 startPos = _player.getArrowSpawn();
		_World *= XMMatrixTranslation(-startPos.x, -startPos.y + 21, -startPos.z);
		//rotate
		_World *= XMMatrixRotationZ(-angleInRadians);
		//translate to current position and down by radius of rotation
		const XMFLOAT3 currentPos = _player.getArrowPos();
		_World *= XMMatrixTranslation(currentPos.x, currentPos.y - 21, currentPos.z);
	}
}
//Set depth stencil
void D3DFramework::drawDepthStencil(const std::string& type) {
	if (type == "fire") {
		_pImmediateContext->OMSetDepthStencilState(_pDepthStencilState_particle, 1);
	}
	else if (type == "sky") {
		_pImmediateContext->OMSetDepthStencilState(_pDepthStencilState_sky, 1);
	}
	else {
		_pImmediateContext->OMSetDepthStencilState(_pDepthStencilState, 1);
	}
}
//Set blend state
void D3DFramework::drawBlend(const std::string& type) {
	const float blendFactor[4] = { 0.5,0.5,0.5,0.5 };
	const UINT sampleMask = 0xffffffff;
	if (type == "fire") {
		_pImmediateContext->OMSetBlendState(_pBlendState, blendFactor, sampleMask);
	}
	else {
		_pImmediateContext->OMSetBlendState(_pDefaultBlend, blendFactor, sampleMask);
	}
}
//Set rasterizer
void D3DFramework::drawRasterizer(const std::string& type) {
	if (type == "sky") {
		_pImmediateContext->RSSetState(_pRast_sky);
	}
	else if (type == "fire") {
		_pImmediateContext->RSSetState(_pRast_particle);
	}
	else {
		_pImmediateContext->RSSetState(_pRast_default);
	}
}
//Set pixel and vertex shader
void D3DFramework::drawShader(const std::string& type) {
	//vertex/pixel shader set
	if (type == "phongSphere") {
		_pImmediateContext->VSSetShader(_pVertexShader_phong, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_phong, nullptr, 0);
	}
	else if (type == "gouradSphere") {
		_pImmediateContext->VSSetShader(_pVertexShader_gourad, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_gourad, nullptr, 0);
	}
	else if (type == "fire") {
		_pImmediateContext->VSSetShader(_pVertexShader_particle, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_particle, nullptr, 0);
	}
	else if (type == "sky") {
		_pImmediateContext->VSSetShader(_pVertexShader_sky, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_sky, nullptr, 0);
	}
	else if (type == "arrow") {
		_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	}
	else if (type == "bumpCube") {
		_pImmediateContext->VSSetShader(_pVertexShader_bump, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_bump, nullptr, 0);
	}
	else if (type == "toonSphere") {
		_pImmediateContext->VSSetShader(_pVertexShader_toon, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_toon, nullptr, 0);
	}
	else if (type == "mirrorCube") {
		_pImmediateContext->VSSetShader(_pVertexShader_mirror, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_mirror, nullptr, 0);
	}
	//textures
	else if (type == "cliff" || type == "ground" || type == "platform" || type == "player") {
		_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_texture, nullptr, 0);
	}
	//default is phong
	else {
		_pImmediateContext->VSSetShader(_pVertexShader_phong, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader_phong, nullptr, 0);
	}
}

//Gui drawing on each frame
void D3DFramework::guiFrame() const {
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
	bool window = true;
	//Esc
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
	ImGui::Begin("Esc", &window, windowFlags);
	ImGui::Text("Esc - Exit");
	ImGui::End();
	//rightjump/leftjump
	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImVec2 windowPos = ImVec2(0, displaySize.y);
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(177,29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(0.0f,1.0f));
	ImGui::Begin("longJump", &window, windowFlags);
	ImGui::Text("A/D + Space - Long Jump");
	ImVec2 lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
	//space
	windowPos.y -= lastWindowSize.y;
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(100, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(0.0f, 1.0f));
	ImGui::Begin("space", &window, windowFlags);
	ImGui::Text("Space - Jump");
	lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
	//rightjump/leftjump
	windowPos.y -= lastWindowSize.y;
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(121, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(0.0f, 1.0f));
	ImGui::Begin("wasd", &window, windowFlags);
	ImGui::Text("WASD - move/look");
	ImGui::End();
	//Reset
	windowPos = ImVec2(displaySize.x, 0);
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(79, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(1.0f, 0.0f));
	ImGui::Begin("reset", &window, windowFlags);
	ImGui::Text("R - reset");
	lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
	//time
	windowPos.y += lastWindowSize.y;
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(339, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(1.0f, 0.0f));
	ImGui::Begin("time", &window, windowFlags);
	ImGui::Text("<> (,.) - decrease/increase time factor: %.3f", _timeFactor);
	lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
	//zoom
	windowPos.y += lastWindowSize.y;
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(290, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(1.0f, 0.0f));
	ImGui::Begin("zoom", &window, windowFlags);
	ImGui::Text("-+ (-=) - decrease/increase zoom: %.3f", -80.0f/_currentDistance);
	lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
	//collectables
	windowPos.y += lastWindowSize.y;
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(163, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(1.0f, 0.0f));
	ImGui::Begin("collect", &window, windowFlags);
	ImGui::Text("Items collected: %d/12", _objectsCollected);
	lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
	//Spotlight
	windowPos = ImVec2(displaySize.x, displaySize.y);
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(191, 29), ImGuiCond_Once);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once, ImVec2(1.0f, 1.0f));
	ImGui::Begin("spotlight", &window, windowFlags);
	ImGui::Text("T - toggle sun/flashlight");
	lastWindowSize = ImGui::GetWindowSize();
	ImGui::End();
}

HRESULT D3DFramework::createTextures() {
	ID3D11ShaderResourceView* pCliffTexture = nullptr;
	ID3D11ShaderResourceView* pGrassTexture = nullptr;
	ID3D11ShaderResourceView* pSteelTexture = nullptr;
	ID3D11ShaderResourceView* pBrushedSteelTexture = nullptr;
	ID3D11ShaderResourceView* pSkyTexture = nullptr;
	ID3D11ShaderResourceView* pRockNormal = nullptr;
	ID3D11ShaderResourceView* pRockTexture = nullptr;
	ID3D11SamplerState* pBasicSampler = nullptr;
	ID3D11SamplerState* pSkySampler = nullptr;

	//Skybox texture
	std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / "Textures";
	std::wstring filename = (tempDir / "Skymap.dds").wstring();
	
	auto hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pSkyTexture);
	if (FAILED(hr)) {
		return hr;
	}
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = _pd3dDevice->CreateSamplerState(&sampDesc, &pSkySampler);

	filename = (tempDir / "cliffs.dds").wstring();
	hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pCliffTexture);
	if (FAILED(hr)) {
		return hr;
	}

	filename = (tempDir / "grass.dds").wstring();
	hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pGrassTexture);
	if (FAILED(hr)) {
		return hr;
	}

	filename = (tempDir / "steel.dds").wstring();
	hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pSteelTexture);
	if (FAILED(hr)) {
		return hr;
	}

	filename = (tempDir / "brushedSteel.dds").wstring();
	hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pBrushedSteelTexture);
	if (FAILED(hr)) {
		return hr;
	}

	filename = (tempDir / "rockNormal.dds").wstring();
	hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pRockNormal);
	if (FAILED(hr)) {
		return hr;
	}

	filename = (tempDir / "stones.DDS").wstring();
	hr = CreateDDSTextureFromFile(_pd3dDevice, filename.c_str(), nullptr, &pRockTexture);
	if (FAILED(hr)) {
		return hr;
	}

	ID3D11SamplerState* pSamplerStates[] = { pBasicSampler, pSkySampler };
	ID3D11ShaderResourceView* pShaderResourceViews[] = { pSkyTexture, pCliffTexture, pGrassTexture, pSteelTexture, pBrushedSteelTexture, pRockNormal, pRockTexture };

	//set samplers and shaders
	_pImmediateContext->PSSetSamplers(0, 2, pSamplerStates);
	_pImmediateContext->PSSetShaderResources(0, 7, pShaderResourceViews);

	//release pointers
	if (pBasicSampler) pBasicSampler->Release();
	if (pSkySampler) pSkySampler->Release();
	if (pSkyTexture) pSkyTexture->Release();
	if (pCliffTexture) pCliffTexture->Release();
	if (pGrassTexture) pGrassTexture->Release();
	if (pSteelTexture) pSteelTexture->Release();
	if (pBrushedSteelTexture) pBrushedSteelTexture->Release();
	if (pRockNormal) pRockNormal->Release();
	if (pRockTexture) pRockTexture->Release();

	return S_OK;
}

HRESULT D3DFramework::createShaders() {
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	auto numElements = static_cast<UINT>(ARRAYSIZE(layout));

#ifdef COMPILE_CSO
	// Compile the vertex shader
	CComPtr <ID3DBlob> pVSBlob;
	auto hr = compileShaderFromFile(SIMULATIONFX, "VS", VS50, &pVSBlob);
	if (FAILED(hr)) {
		return hr;
	}
	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the gourad vertex shader
	CComPtr <ID3DBlob> pVSBlob_gourad;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_gourad", VS50, &pVSBlob_gourad);
	if (FAILED(hr)) {
		return hr;
	}
	//create the gourad vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_gourad->GetBufferPointer(), pVSBlob_gourad->GetBufferSize(), nullptr, &_pVertexShader_gourad);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the phong vertex shader
	CComPtr <ID3DBlob> pVSBlob_phong;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_phong", VS50, &pVSBlob_phong);
	if (FAILED(hr)) {
		return hr;
	}
	//create the phong vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_phong->GetBufferPointer(), pVSBlob_phong->GetBufferSize(), nullptr, &_pVertexShader_phong);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the particel vertex shader
	CComPtr <ID3DBlob> pVSBlob_particle;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_particle", VS50, &pVSBlob_particle);
	if (FAILED(hr)) {
		return hr;
	}
	//create the particle vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_particle->GetBufferPointer(), pVSBlob_particle->GetBufferSize(), nullptr, &_pVertexShader_particle);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the skybox vertex shader
	CComPtr <ID3DBlob> pVSBlob_sky;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_sky", VS50, &pVSBlob_sky);
	if (FAILED(hr)) {
		return hr;
	}
	//create the skybox vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_sky->GetBufferPointer(), pVSBlob_sky->GetBufferSize(), nullptr, &_pVertexShader_sky);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the bumpmap vertex shader
	CComPtr <ID3DBlob> pVSBlob_bump;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_bump", VS50, &pVSBlob_bump);
	if (FAILED(hr)) {
		return hr;
	}
	//create the bumpmap vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_bump->GetBufferPointer(), pVSBlob_bump->GetBufferSize(), nullptr, &_pVertexShader_bump);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the toon vertex shader
	CComPtr <ID3DBlob> pVSBlob_toon;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_toon", VS50, &pVSBlob_toon);
	if (FAILED(hr)) {
		return hr;
	}
	//create the toon vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_toon->GetBufferPointer(), pVSBlob_toon->GetBufferSize(), nullptr, &_pVertexShader_toon);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the toon vertex shader
	CComPtr <ID3DBlob> pVSBlob_mirror;
	hr = compileShaderFromFile(SIMULATIONFX, "VS_mirror", VS50, &pVSBlob_mirror);
	if (FAILED(hr)) {
		return hr;
	}
	//create the toon vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob_mirror->GetBufferPointer(), pVSBlob_mirror->GetBufferSize(), nullptr, &_pVertexShader_mirror);
	if (FAILED(hr)) {
		return hr;
	}

	// Create the input layout
	//assumes that VS requests every parameter that every other VS_ requests
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_pVertexLayout);
	if (FAILED(hr))
		return hr;
#else
	{
		const std::string fileName{ "..\\Debug\\Simulation_VS.cso" };
		std::ifstream fin(fileName, std::ios::binary);
		if (!fin) {
			MessageBox(nullptr, L"The CSO file cannot be found.", L"Error", MB_OK);
			return E_FAIL;
		}
		std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});

		hr = _pd3dDevice->CreateVertexShader(&byteCode[0], byteCode.size(), nullptr, &_pVertexShader);
		if (FAILED(hr)) {
			return hr;
		}

		// Create the input layout
		hr = _pd3dDevice->CreateInputLayout(layout, numElements, &byteCode[0], byteCode.size(), &_pVertexLayout);
		if (FAILED(hr))
			return hr;
	}
#endif

	// Set the input layout
	_pImmediateContext->IASetInputLayout(_pVertexLayout);

#ifdef COMPILE_CSO
	// Compile the pixel shader
	CComPtr <ID3DBlob> pPSBlob;
	hr = compileShaderFromFile(SIMULATIONFX, "PS", PS50, &pPSBlob);
	if (FAILED(hr)) {
		return hr;
	}
	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the phong pixel shader
	CComPtr <ID3DBlob> pPSBlob_gourad;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_gourad", PS50, &pPSBlob_gourad);
	if (FAILED(hr)) {
		return hr;
	}
	//create the phong pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_gourad->GetBufferPointer(), pPSBlob_gourad->GetBufferSize(), nullptr, &_pPixelShader_gourad);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the phong pixel shader
	CComPtr <ID3DBlob> pPSBlob_phong;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_phong", PS50, &pPSBlob_phong);
	if (FAILED(hr)) {
		return hr;
	}
	//create the phong pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_phong->GetBufferPointer(), pPSBlob_phong->GetBufferSize(), nullptr, &_pPixelShader_phong);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the particle pixel shader
	CComPtr <ID3DBlob> pPSBlob_particle;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_particle", PS50, &pPSBlob_particle);
	if (FAILED(hr)) {
		return hr;
	}
	//create the particle pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_particle->GetBufferPointer(), pPSBlob_particle->GetBufferSize(), nullptr, &_pPixelShader_particle);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the sky pixel shader
	CComPtr <ID3DBlob> pPSBlob_sky;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_sky", PS50, &pPSBlob_sky);
	if (FAILED(hr)) {
		return hr;
	}
	//create the sky pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_sky->GetBufferPointer(), pPSBlob_sky->GetBufferSize(), nullptr, &_pPixelShader_sky);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the texture pixel shader
	CComPtr <ID3DBlob> pPSBlob_texture;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_texture", PS50, &pPSBlob_texture);
	if (FAILED(hr)) {
		return hr;
	}
	//create the texture pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_texture->GetBufferPointer(), pPSBlob_texture->GetBufferSize(), nullptr, &_pPixelShader_texture);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the bump pixel shader
	CComPtr <ID3DBlob> pPSBlob_bump;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_bump", PS50, &pPSBlob_bump);
	if (FAILED(hr)) {
		return hr;
	}
	//create the bump pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_bump->GetBufferPointer(), pPSBlob_bump->GetBufferSize(), nullptr, &_pPixelShader_bump);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the toon pixel shader
	CComPtr <ID3DBlob> pPSBlob_toon;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_toon", PS50, &pPSBlob_toon);
	if (FAILED(hr)) {
		return hr;
	}
	//create the toon pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_toon->GetBufferPointer(), pPSBlob_toon->GetBufferSize(), nullptr, &_pPixelShader_toon);
	if (FAILED(hr)) {
		return hr;
	}

	//compile the mirror pixel shader
	CComPtr <ID3DBlob> pPSBlob_mirror;
	hr = compileShaderFromFile(SIMULATIONFX, "PS_mirror", PS50, &pPSBlob_mirror);
	if (FAILED(hr)) {
		return hr;
	}
	//create the mirror pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob_mirror->GetBufferPointer(), pPSBlob_mirror->GetBufferSize(), nullptr, &_pPixelShader_mirror);
	if (FAILED(hr)) {
		return hr;
	}

#else
	{
		const std::string fileName{ "..\\Debug\\Simulation_PS.cso" };
		std::ifstream fin(fileName, std::ios::binary);
		if (!fin) {
			MessageBox(nullptr, L"The CSO file cannot be found.", L"Error", MB_OK);
			return E_FAIL;
		}
		std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});

		hr = _pd3dDevice->CreatePixelShader(&byteCode[0], byteCode.size(), nullptr, &_pPixelShader);
		if (FAILED(hr)) {
			return hr;
		}
	}
#endif
	return S_OK;
}