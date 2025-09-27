#include "pch.h"
#include "AdvancedSetup.h"

D3D11_TEXTURE2D_DESC AdvancedSetup::basicDS(int width, int height){
	//default depth stencil
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = static_cast<UINT>(width);
	descDepth.Height = static_cast<UINT>(height);
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	return descDepth;
}

D3D11_DEPTH_STENCIL_VIEW_DESC AdvancedSetup::basicDSV(D3D11_TEXTURE2D_DESC descDepth){
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	
	return descDSV;
}

D3D11_DEPTH_STENCIL_DESC AdvancedSetup::basicDSS(){
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));
	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	return depthStencilStateDesc;
}

HRESULT AdvancedSetup::setupDepthStencil(CComPtr<ID3D11Device>& device, CComPtr<ID3D11Texture2D>& DSPtr, CComPtr<ID3D11DepthStencilView>& DSVPtr, 
										 CComPtr<ID3D11DepthStencilState>& DSSPtr, int windowWidth, int windowHeight){
	D3D11_TEXTURE2D_DESC tempDS = basicDS(windowWidth, windowHeight);
	HRESULT hr = device->CreateTexture2D(&tempDS, nullptr, &DSPtr);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC tempDSV = basicDSV(tempDS);
	hr = device->CreateDepthStencilView(DSPtr, &tempDSV, &DSVPtr);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_DESC tempDSS = basicDSS();
	hr = device->CreateDepthStencilState(&tempDSS, &DSSPtr);

	return hr;
}

HRESULT AdvancedSetup::setupRasterizer(CComPtr<ID3D11Device>& device, CComPtr<ID3D11RasterizerState>& rast) {
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	return device->CreateRasterizerState(&rasterDesc, &rast);
}

HRESULT AdvancedSetup::setupTextues(CComPtr<ID3D11Device>& device, CComPtr<ID3D11DeviceContext>& context) {
	std::filesystem::path tempDir = helpers::getRuntimeFilepath() / "Assets" / "Textures";
	if (!std::filesystem::exists(tempDir))
		std::filesystem::create_directories(tempDir);
	
	std::filesystem::path myPath = tempDir / "Scales.dds";
	if (!std::filesystem::exists(myPath)) {
		std::filesystem::path logFile = helpers::getRuntimeFilepath() / "errorLog.txt";
		std::ofstream ofs(logFile.string());
		ofs << "Error loading texture file";
	}
	else {
		std::filesystem::path logFile = helpers::getRuntimeFilepath() / "errorLog.txt";
		std::ofstream ofs(logFile.string());
		ofs << "File located successfully";
	}
	std::wstring filename = myPath.wstring();

	ID3D11ShaderResourceView* defaultTexture;
	auto hr = DirectX::CreateDDSTextureFromFile(device, filename.c_str(), nullptr, &defaultTexture);
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
	ID3D11SamplerState* defaultSampler;
	hr = device->CreateSamplerState(&sampDesc, &defaultSampler);
	if (FAILED(hr)) {
		return hr;
	}

	ID3D11SamplerState* pSamplerStates[] = {defaultSampler};
	ID3D11ShaderResourceView* pShaderResourceViews[] = { defaultTexture };

	context->PSSetSamplers(0, 1, pSamplerStates);
	context->PSSetShaderResources(0, 1, pShaderResourceViews);
	
	if (defaultTexture) defaultTexture->Release();
	if (defaultSampler) defaultSampler->Release();

	return S_OK;
}