//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>
#include <DirectXMath.h>

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.
using namespace DirectX;

namespace RenderLightHelper
{

	struct DirLight
	{
		DirLight() { ZeroMemory(this, sizeof(this));}

		XMFLOAT3 Color;
		float pad1;
		XMFLOAT3 Direction;
		float pad2; // Pad the last float so we can set an array of lights if we wanted.
	};

	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT3 Color;
		float pad1;

		XMFLOAT3 Position;
		float Range = 100.0;
	};

	struct Material
	{
		Material() {}

		XMFLOAT3 albedo = XMFLOAT3(0.5, 0.5, 0.5);
		float roughness = 0.8;
		float metallic = 0.0;
	};
}

#endif // LIGHTHELPER_H