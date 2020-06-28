#pragma once

#include "d3dUtil.h"

class SSAOMap
{
public:
	SSAOMap(ID3D11Device* device, UINT width, UINT height);
	~SSAOMap();
	void SetNormalDepthRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv);
	void SetRenderSSAORenderTarget(ID3D11DeviceContext* dc);

	ID3D11ShaderResourceView* getRandomVectorSRV() {
		return mRandomVectorSRV;
	}

	ID3D11ShaderResourceView* getNormalDepthMapSRV() {
		return mNormalDepthMapSRV;
	}

	std::vector<XMFLOAT4> getOffsets() { return mOffsets; }

private:
	void BuildOffsetVectors();
	void BuildRandomVectorTexture(ID3D11Device* device);

	ID3D11DeviceContext* context;
	UINT mWidth;
	UINT mHeight;
	std::vector<XMFLOAT4> mOffsets;

	ID3D11ShaderResourceView* mRandomVectorSRV;
	ID3D11ShaderResourceView* mNormalDepthMapSRV;
	ID3D11RenderTargetView* mNormalDepthRTV;

	ID3D11ShaderResourceView* mSSAOMapSRV1;
	ID3D11RenderTargetView* mSSAOMapRTV1;

	ID3D11ShaderResourceView* mSSAOMapSRV2;
	ID3D11RenderTargetView* mSSAOMapRTV2;

	D3D11_VIEWPORT mSSAOViewport;
};

