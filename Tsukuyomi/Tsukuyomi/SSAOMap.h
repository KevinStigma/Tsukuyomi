#pragma once

#include "d3dUtil.h"

class SSAOMap
{
public:
	SSAOMap(ID3D11Device* device, UINT width, UINT height);
	~SSAOMap();
	void SetNormalDepthRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv);

private:
	ID3D11DeviceContext* context;
	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* mNormalDepthMapSRV;
	ID3D11RenderTargetView* mNormalDepthRTV;

	ID3D11ShaderResourceView* mSSAOMapSRV1;
	ID3D11RenderTargetView* mSSAOMapRTV1;

	ID3D11ShaderResourceView* mSSAOMapSRV2;
	ID3D11RenderTargetView* mSSAOMapRTV2;

	D3D11_VIEWPORT mViewport;
};

