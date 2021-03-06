#pragma once
//***************************************************************************************
// ShadowMap.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper class for implementing shadows via shadow mapping algorithm.
//***************************************************************************************

#ifndef SHADOW_MAPPER_H
#define SHADOW_MAPPER_H

#include "d3dUtil.h"

class ShadowMap
{
public:
	ShadowMap(ID3D11Device* device, UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView* DepthMapSRV();

	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);

private:
	UINT mWidth;
	UINT mHeight;

	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;

	D3D11_VIEWPORT mViewport;
};

#endif // SHADOW_MAPPER_H