#pragma once

#include "d3dUtil.h"

class SSAOMap
{
public:
	SSAOMap(ID3D11Device* device, int width, int height);
	~SSAOMap();
	void SetNormalDepthRenderTarget(ID3D11DeviceContext* dc, ID3D11DepthStencilView* dsv);
	void SetRenderSSAORenderTarget(ID3D11DeviceContext* dc);
	void blurSSAOMap(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* inputSRV, ID3D11RenderTargetView* outputRTV, ID3D11Buffer*quadVertexBuffer, ID3D11Buffer* quadIndexBuffer, bool horizontal);
	void blurSSAOMap(ID3D11DeviceContext*dc, ID3D11Buffer*quadVertexBuffer, ID3D11Buffer* quadIndexBuffer, int blurCount);

	ID3D11ShaderResourceView* getRandomVectorSRV() {
		return mRandomVectorSRV;
	}

	ID3D11ShaderResourceView* getNormalDepthMapSRV() {
		return mNormalDepthMapSRV;
	}

	ID3D11ShaderResourceView* getSSAOMapSRV() {
		return mSSAOMapSRV1;
	}

	ID3D11ShaderResourceView* getDepthMapSRV() {
		return mDepthMapSRV;
	}

	std::vector<XMFLOAT4> getOffsets() { return mOffsets; }

private:
	void BuildOffsetVectors();
	void BuildRandomVectorTexture(ID3D11Device* device);

	ID3D11DeviceContext* context;
	std::vector<XMFLOAT4> mOffsets;

	ID3D11ShaderResourceView* mRandomVectorSRV;
	ID3D11ShaderResourceView* mNormalDepthMapSRV;
	ID3D11RenderTargetView* mNormalDepthRTV;

	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;

	ID3D11ShaderResourceView* mSSAOMapSRV1;
	ID3D11RenderTargetView* mSSAOMapRTV1;

	ID3D11ShaderResourceView* mSSAOMapSRV2;
	ID3D11RenderTargetView* mSSAOMapRTV2;

	D3D11_VIEWPORT mSSAOViewport;
};

