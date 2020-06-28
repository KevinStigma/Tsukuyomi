#pragma once

#include "d3dUtil.h"

class SSAOMap
{
public:
	SSAOMap(ID3D11Device* device, UINT width, UINT height);
	~SSAOMap();

private:
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

