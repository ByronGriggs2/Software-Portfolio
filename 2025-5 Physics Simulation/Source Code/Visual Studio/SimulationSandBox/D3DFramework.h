#pragma once

//#include <Windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <atlbase.h>
#include <vector>
#include <memory>
#include "PtrPreset.h"
#include "NetCompatibleGameObject.h"
#include "NetCompatibleImGuiState.h"

#define COMPILE_CSO
namespace helpers {
	struct RenderedObjectStruct;
}

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMMATRIX normalMatrix;
	DirectX::XMVECTOR lightPos;
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR baseColor;
	int texType;
	float padding[3];
};

/////////////////////
// Singleton

class D3DFramework final {
	HINSTANCE _hInst = nullptr;
	HWND _hWnd = nullptr;
	D3D_DRIVER_TYPE _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL _featureLevel = D3D_FEATURE_LEVEL_11_1;
	CComPtr <ID3D11Device> _pd3dDevice;
	CComPtr <ID3D11Device> _pd3dDevice1;
	CComPtr <ID3D11DeviceContext> _pImmediateContext;
	CComPtr <ID3D11DeviceContext> _pImmediateContext1;
	CComPtr <IDXGISwapChain1> _swapChain;
	CComPtr <IDXGISwapChain1> _swapChain1;
	CComPtr <ID3D11RenderTargetView> _pRenderTargetView;

	CComPtr <ID3D11Texture2D> _pDepthStencil_default;
	CComPtr <ID3D11DepthStencilView> _pDepthStencilView_default;
	PtrPreset _ptrPresets[2];

	CComPtr <ID3D11Buffer> _pConstantBuffer;
	DirectX::XMMATRIX _World = {};
	DirectX::XMMATRIX _View = {};
	DirectX::XMMATRIX _Projection = {};

	LARGE_INTEGER _frequency;
	LARGE_INTEGER _frameStart, _frameEnd;

	D3DFramework() = default;
public:
	D3DFramework(D3DFramework&) = delete;
	D3DFramework(D3DFramework&&) = delete;
	D3DFramework& operator=(const D3DFramework&) = delete;
	D3DFramework& operator=(const D3DFramework&&) = delete;
	~D3DFramework();

	static std::shared_ptr<D3DFramework> getInstance() { 
		static std::shared_ptr<D3DFramework> instance(new D3DFramework);
		return instance;
	}
	// Magic
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Initialization
	HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
	static HRESULT compileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT initDevice();
	HRESULT createShaders();
	void threadingInit() const;

	void render();
private:
		// Shared memory cache
	// Set to true by this thread. Tells network thread to reset peers. Then set to false
	// by network thread
	bool _resetPeers = false;
	bool _renderReset = false;
	bool _physicsReset = false;
	bool _networkReset = false;

	float _actualSimulationSpeed = 200.0f;
	float _actualNetworkSpeed = 20.0f;
	std::vector<NetCompatibleGameObject> _objectsToDraw;
	std::shared_ptr<float[]> _defaultColor = std::make_shared<float[]>(4);
	DirectX::XMVECTOR _cameraPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR _lightPosition = DirectX::XMVectorZero();
	NetCompatibleImGuiState _ImGuiState;

	bool _renderResetThisLoop = false;
	
	void getInputs();
	helpers::RenderedObjectStruct calculateRenderedObjects() const;
	void drawSingle(const DirectX::XMVECTOR& lightPos, const DirectX::XMVECTOR& Eye, const NetCompatibleGameObject& obj);
	void endOfFrame();
	void sleepUntilNextFrame();

#ifdef _DEBUG
	void DebugVertexBuffer(const CComPtr<ID3D11Buffer>& vertexBuffer, ID3D11DeviceContext* context);
#endif
};