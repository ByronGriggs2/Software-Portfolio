#pragma once

#include <d3d11.h>
#include <atlbase.h>
#include <memory>

/////////////////////
// Static class

class AdvancedSetup final
{
	static D3D11_TEXTURE2D_DESC basicDS(int width, int height);
	static D3D11_DEPTH_STENCIL_VIEW_DESC basicDSV(D3D11_TEXTURE2D_DESC descDepth);
	static D3D11_DEPTH_STENCIL_DESC basicDSS();

public:
	AdvancedSetup() = delete;
	AdvancedSetup(const AdvancedSetup& other) = delete;
	AdvancedSetup& operator=(const AdvancedSetup& other) = delete;
	AdvancedSetup(const AdvancedSetup&& other) = delete;
	AdvancedSetup& operator=(const AdvancedSetup&&) = delete;

	static HRESULT setupDepthStencil(CComPtr<ID3D11Device>& device, CComPtr<ID3D11Texture2D>& DSDPtr, CComPtr<ID3D11DepthStencilView>& DSVPtr, CComPtr<ID3D11DepthStencilState>& DSSPtr, int windowWidth, int windowHeight);
	static HRESULT setupRasterizer(CComPtr<ID3D11Device>& device, CComPtr<ID3D11RasterizerState>& rast);
	static HRESULT setupTextues(CComPtr<ID3D11Device>& device, CComPtr<ID3D11DeviceContext>& context);
};

