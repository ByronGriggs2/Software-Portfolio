#pragma once

#include <atlbase.h>
#include <vector>
#include "d3d11.h"

// The physics and network threads choose what preset to use for their objects (and put it in
// shared memory). They cannot define the presets, however, that is done by the render thread.
// The only thing in the shared memory should be an enum. This actual struct is just in render.
struct PtrPreset {
	CComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	CComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	CComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	CComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
	CComPtr<ID3D11RasterizerState> _rasterizer = nullptr;
};