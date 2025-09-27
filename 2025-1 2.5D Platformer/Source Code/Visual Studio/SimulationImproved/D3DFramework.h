#pragma once

//Windows.h 
#include <Windows.h>
#include <fstream>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <vector>
#include <iostream>
//for std::unique_ptr
#include <memory>
#include <chrono>
#include <thread>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "DDSTextureLoader.h"
#include "PlayerWrap.h"
#include "CompleteObject.h"


using namespace DirectX;

#define COMPILE_CSO

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

//for detecting if player is on platform
struct hitBox {
	XMFLOAT3 p1, p2;
	hitBox() {
		p1 = XMFLOAT3(-9999.0, 0.0, -9999.0);
		p2 = XMFLOAT3(9999.0, 0.0, 9999.0);
	}
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMVECTOR lightPos;
	XMVECTOR cameraPos;
	//used for fire particles
	float time;
	//treated as a bool, indicates if flashlight or sun is on
	float spotlightCB;
	//treates as an enum, specifies which texture to use
	int texType;
	float buffer;
};

class D3DFramework final {

	HINSTANCE _hInst = nullptr;
	HWND _hWnd = nullptr;
	D3D_DRIVER_TYPE _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL _featureLevel = D3D_FEATURE_LEVEL_11_1;
	CComPtr <ID3D11Device> _pd3dDevice;
	CComPtr <ID3D11Device1> _pd3dDevice1;
	CComPtr <ID3D11DeviceContext> _pImmediateContext;
	CComPtr <ID3D11DeviceContext1> _pImmediateContext1;
	CComPtr <IDXGISwapChain1> _swapChain;
	CComPtr <IDXGISwapChain1> _swapChain1;
	CComPtr <ID3D11RenderTargetView> _pRenderTargetView;

	CComPtr <ID3D11VertexShader> _pVertexShader;
	CComPtr <ID3D11VertexShader> _pVertexShader_phong;
	CComPtr <ID3D11VertexShader> _pVertexShader_gourad;
	CComPtr <ID3D11VertexShader> _pVertexShader_particle;
	CComPtr <ID3D11VertexShader> _pVertexShader_sky;
	CComPtr <ID3D11VertexShader> _pVertexShader_bump;
	CComPtr <ID3D11VertexShader> _pVertexShader_toon;
	CComPtr <ID3D11VertexShader> _pVertexShader_mirror;
	CComPtr <ID3D11PixelShader> _pPixelShader;
	CComPtr <ID3D11PixelShader> _pPixelShader_phong;
	CComPtr <ID3D11PixelShader> _pPixelShader_gourad;
	CComPtr <ID3D11PixelShader> _pPixelShader_particle;
	CComPtr <ID3D11PixelShader> _pPixelShader_sky;
	CComPtr <ID3D11PixelShader> _pPixelShader_texture;
	CComPtr <ID3D11PixelShader> _pPixelShader_bump;
	CComPtr <ID3D11PixelShader> _pPixelShader_toon;
	CComPtr <ID3D11PixelShader> _pPixelShader_mirror;

	CComPtr <ID3D11InputLayout> _pVertexLayout;
	CComPtr <ID3D11Buffer> _pConstantBuffer;
	XMMATRIX _World = {};
	XMMATRIX _View = {};
	XMMATRIX _Projection = {};

	float _rotation = 0.0f;
	float _currentDistance = -80.0f;
	//used for spotlightCB
	float _spotlightOn = 0.0f;
	float _timeFactor = 1.0f;
	//used to debounce buttons
	bool _spotlightDebounce = false;
	bool _timeDownDebounce = false;
	bool _timeUpDebounce = false;

	CComPtr <ID3D11Texture2D> _pDepthStencil;
	CComPtr <ID3D11DepthStencilView> _pDepthStencilView;
	CComPtr <ID3D11DepthStencilState> _pDepthStencilState;
	CComPtr <ID3D11DepthStencilState> _pDepthStencilState_particle;
	CComPtr <ID3D11DepthStencilState> _pDepthStencilState_sky;
	CComPtr <ID3D11RasterizerState> _pRast_default;
	CComPtr <ID3D11RasterizerState> _pRast_particle;
	CComPtr <ID3D11RasterizerState> _pRast_sky;
	CComPtr <ID3D11BlendState> _pBlendState;
	CComPtr <ID3D11BlendState> _pDefaultBlend;

	//contains all objects except Player and Arrow
	std::vector<CompleteObject> _objectsToDraw;
	//contains Player and Arrow
	PlayerWrap _player;
	int _objectsCollected = 0;
	std::vector<hitBox> _solidGround;

	static std::unique_ptr<D3DFramework> _instance;
public:

	D3DFramework() = default;
	D3DFramework(D3DFramework&) = delete;
	D3DFramework(D3DFramework&&) = delete;
	D3DFramework& operator=(const D3DFramework&) = delete;
	D3DFramework& operator=(const D3DFramework&&) = delete;
	~D3DFramework();
	
	static D3DFramework& getInstance() { return *_instance; }

	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
	static HRESULT compileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	HRESULT initDevice();
	//this is where the vertex and index buffers are set, once per object per program
	void bufferAll();
	void bufferSingle(CompleteObject&);

	void render();
	void guiFrame() const;
	void physics();
	bool abovePlatform();
	void controls();

	//These are big chunks of related code taken out from initDevice to reduce its size
	HRESULT createTextures();
	HRESULT createShaders();

	//the drawSingle function cut up into smaller functions
	void drawSingle(const XMVECTOR& lightPos, const XMVECTOR& Eye, float time, const CompleteObject& obj);
	void drawRotation(Rotation, const CompleteObject&);
	int drawTextureType(const std::string&) const;
	void drawPlayerMove(const std::string&);
	void drawDepthStencil(const std::string&);
	void drawBlend(const std::string&);
	void drawRasterizer(const std::string&);
	void drawShader(const std::string&);
};