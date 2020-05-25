#pragma once
#include <DirectXMath.h>

using namespace DirectX;
struct ShadowTransform
{
	XMFLOAT4X4 shadowTransMat;
	XMFLOAT4X4 lightViewTransMat;
	XMFLOAT4X4 lightProjTransMat;
};
