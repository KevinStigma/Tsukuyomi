#include "SSAOMap.h"
#include "common.h"
#include "Effects/Effects.h"
#include "Vertex.h"

SSAOMap::SSAOMap(ID3D11Device* device, int width, int height):mNormalDepthMapSRV(nullptr), mNormalDepthRTV(nullptr)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* normalDepthMap = 0;
	HRESULT hr = device->CreateTexture2D(&texDesc, 0, &normalDepthMap);

	device->CreateShaderResourceView(normalDepthMap, 0, &mNormalDepthMapSRV);
	device->CreateRenderTargetView(normalDepthMap, 0, &mNormalDepthRTV);

	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	
	ID3D11Texture2D* depthMap = 0;
	hr = device->CreateTexture2D(&depthDesc, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthMap, &dsvDesc, &mDepthMapDSV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV);

	// view saves a reference.
	ReleaseCOM(normalDepthMap);
	ReleaseCOM(depthMap);

	texDesc.Width = width * 0.5;
	texDesc.Height = height * 0.5;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;

	ID3D11Texture2D* ssaoMap = 0;
	hr = device->CreateTexture2D(&texDesc, 0, &ssaoMap);
	device->CreateShaderResourceView(ssaoMap, 0, &mSSAOMapSRV1);
	device->CreateRenderTargetView(ssaoMap, 0, &mSSAOMapRTV1);

	ID3D11Texture2D* ssaoMap2 = 0;
	hr = device->CreateTexture2D(&texDesc, 0, &ssaoMap2);
	device->CreateShaderResourceView(ssaoMap2, 0, &mSSAOMapSRV2);
	device->CreateRenderTargetView(ssaoMap2, 0, &mSSAOMapRTV2);

	ReleaseCOM(ssaoMap);
	ReleaseCOM(ssaoMap2);

	mSSAOViewport.TopLeftX = 0.0f;
	mSSAOViewport.TopLeftY = 0.0f;
	mSSAOViewport.Width = static_cast<float>(width) * 0.5;
	mSSAOViewport.Height = static_cast<float>(height) * 0.5;
	mSSAOViewport.MinDepth = 0.0f;
	mSSAOViewport.MaxDepth = 1.0f;

	BuildOffsetVectors();
	BuildRandomVectorTexture(device);
}

void SSAOMap::SetNormalDepthRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv)
{
	dc->OMSetRenderTargets(1, &mNormalDepthRTV, mDepthMapDSV);

	// Clear view space normal to (0,0,-1) and clear depth to be very far away.  
	float clearColor[] = { 0.0f, 0.0f, -1.0f, 1e5 };
	dc->ClearRenderTargetView(mNormalDepthRTV, clearColor);
	dc->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void SSAOMap::SetRenderSSAORenderTarget(ID3D11DeviceContext* dc)
{
	ID3D11RenderTargetView* renderTargets[1] = { mSSAOMapRTV1 };
	dc->OMSetRenderTargets(1, renderTargets, 0);
  
	float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->ClearRenderTargetView(mSSAOMapRTV1, clearColor);
	dc->RSSetViewports(1, &mSSAOViewport);
}

void SSAOMap::blurSSAOMap(ID3D11DeviceContext*dc, ID3D11Buffer*quadVertexBuffer, ID3D11Buffer* quadIndexBuffer, int blurCount)
{
	for (int i = 0; i < blurCount; i++)
	{
		blurSSAOMap(dc, mSSAOMapSRV1, mSSAOMapRTV2, quadVertexBuffer, quadIndexBuffer, true);
		blurSSAOMap(dc, mSSAOMapSRV2, mSSAOMapRTV1, quadVertexBuffer, quadIndexBuffer, false);
	}
}

void SSAOMap::blurSSAOMap(ID3D11DeviceContext*dc, ID3D11ShaderResourceView* inputSRV, ID3D11RenderTargetView* outputRTV, ID3D11Buffer*quadVertexBuffer, ID3D11Buffer* quadIndexBuffer, bool horizontal)
{
	ID3D11RenderTargetView* renderTargets[1] = { outputRTV };
	dc->OMSetRenderTargets(1, renderTargets, 0);
	dc->ClearRenderTargetView(outputRTV, reinterpret_cast<const float*>(&Colors::White));
	dc->RSSetViewports(1, &mSSAOViewport);
	
	Effects::SSAOBlurFX->SetTexelWidth(1.0f / mSSAOViewport.Width);
	Effects::SSAOBlurFX->SetTexelHeight(1.0f / mSSAOViewport.Height);
	Effects::SSAOBlurFX->SetNormalDepthMap(mNormalDepthMapSRV);
	Effects::SSAOBlurFX->SetInputImage(inputSRV);

	ID3DX11EffectTechnique* tech;
	if (horizontal)
		tech = Effects::SSAOBlurFX->HorizontalBlurTech;
	else
		tech = Effects::SSAOBlurFX->VerticalBlurTech;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;

	dc->IASetInputLayout(InputLayouts::PosNorTex);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->IASetVertexBuffers(0, 1, &quadVertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(6, 0, 0);
	}
}

void SSAOMap::BuildRandomVectorTexture(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = 256 * sizeof(XMFLOAT3);

	XMFLOAT3 color[256 * 256];
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 256; ++j)
		{
			color[i * 256 + j] = XMFLOAT3(MathHelper::RandF(), MathHelper::RandF(), MathHelper::RandF());;
		}
	}

	initData.pSysMem = color;

	ID3D11Texture2D* tex = 0;
	device->CreateTexture2D(&texDesc, &initData, &tex);

	device->CreateShaderResourceView(tex, 0, &mRandomVectorSRV);

	// view saves a reference.
	ReleaseCOM(tex);
}

void SSAOMap::BuildOffsetVectors()
{
	// Start with 14 uniformly distributed vectors.  We choose the 8 corners of the cube
	// and the 6 center points along each cube face.  We always alternate the points on 
	// opposites sides of the cubes.  This way we still get the vectors spread out even
	// if we choose to use less than 14 samples.

	mOffsets.resize(14);

	// 8 cube corners
	mOffsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	mOffsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	mOffsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	mOffsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	mOffsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	mOffsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	mOffsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	mOffsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = MathHelper::RandF(0.25f, 1.0f);

		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&mOffsets[i]));

		XMStoreFloat4(&mOffsets[i], v);
	}
}

SSAOMap::~SSAOMap()
{
	SAFE_RELEASE(mNormalDepthMapSRV);
	SAFE_RELEASE(mNormalDepthRTV);
	
	SAFE_RELEASE(mSSAOMapSRV1);
	SAFE_RELEASE(mSSAOMapRTV1);

	SAFE_RELEASE(mSSAOMapSRV2);
	SAFE_RELEASE(mSSAOMapRTV2);
	SAFE_RELEASE(mDepthMapDSV);
}