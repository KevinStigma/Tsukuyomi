#include "SSAOMap.h"
#include "common.h"

SSAOMap::SSAOMap(ID3D11Device* device, UINT width, UINT height)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* normalDepthMap = 0;
	HRESULT hr = device->CreateTexture2D(&texDesc, 0, &normalDepthMap);

	device->CreateTexture2D(&texDesc, 0, &normalDepthMap);
	device->CreateShaderResourceView(normalDepthMap, 0, &mNormalDepthMapSRV);
	device->CreateRenderTargetView(normalDepthMap, 0, &mNormalDepthRTV);
	
	// view saves a reference.
	ReleaseCOM(normalDepthMap);

	texDesc.Width = mWidth * 0.5;
	texDesc.Height = mHeight * 0.5;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;

	ID3D11Texture2D* ssaoMap = 0;
	hr = device->CreateTexture2D(&texDesc, 0, &ssaoMap);

	device->CreateTexture2D(&texDesc, 0, &ssaoMap);
	device->CreateShaderResourceView(ssaoMap, 0, &mSSAOMapSRV1);
	device->CreateRenderTargetView(ssaoMap, 0, &mSSAOMapRTV1);

	device->CreateShaderResourceView(ssaoMap, 0, &mSSAOMapSRV2);
	device->CreateRenderTargetView(ssaoMap, 0, &mSSAOMapRTV2);

	ReleaseCOM(ssaoMap);
}

SSAOMap::~SSAOMap()
{
	SAFE_RELEASE(mNormalDepthMapSRV);
	SAFE_RELEASE(mNormalDepthRTV);
	
	SAFE_RELEASE(mSSAOMapSRV1);
	SAFE_RELEASE(mSSAOMapRTV1);

	SAFE_RELEASE(mSSAOMapSRV2);
	SAFE_RELEASE(mSSAOMapRTV2);
}