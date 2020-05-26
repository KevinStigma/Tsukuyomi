#include "Effects.h"
#include "../common.h"
#include <windows.h>
#include <iostream>

#pragma region Effect
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	: mFX(0)
{
	ID3D10Blob * compilationMsgs = 0;
	auto hr=D3DX11CompileEffectFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, D3DCOMPILE_ENABLE_STRICTNESS, 0, device, &mFX, &compilationMsgs);

	if (FAILED(hr))
		std::cout << "compile effect file failed!" << std::endl;
}

Effect::~Effect()
{
	SAFE_DELETE(mFX);
}
#pragma endregion

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	Light1Tech        = mFX->GetTechniqueByName("Light1");
	Light2Tech        = mFX->GetTechniqueByName("Light2");
	Light3Tech        = mFX->GetTechniqueByName("Light3");

	Light0TexTech     = mFX->GetTechniqueByName("Light0Tex");
	Light1TexTech	  = mFX->GetTechniqueByName("Light1Tex");
	Light2TexTech     = mFX->GetTechniqueByName("Light2Tex");
	Light3TexTech     = mFX->GetTechniqueByName("Light3Tex");
	SimpleColorTech	  = mFX->GetTechniqueByName("SimpleColor");
	DebugNormalTech	  = mFX->GetTechniqueByName("DebugNormal");
	CustomLightTech   = mFX->GetTechniqueByName("CustomLight");
	CustomLightShadowTech = mFX->GetTechniqueByName("CustomLightShadow");

	Light1TexAlphaClipFogTech = mFX->GetTechniqueByName("Light1TexAlphaClipFog");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights         = mFX->GetVariableByName("gDirLights");
	PointLights		  = mFX->GetVariableByName("gPointLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap		  = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	ShadowMap		  = mFX->GetVariableByName("gShadowMap")->AsShaderResource();
	ShadowTransform   = mFX->GetVariableByName("gShadowTransform")->AsMatrix();
	TexTransform	  = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
	curDirLightCount = mFX->GetVariableByName("curDirLightCount")->AsScalar();
	curPointLightCount = mFX->GetVariableByName("curPointLightCount")->AsScalar();
	gammaCorrect = mFX->GetVariableByName("gammaCorrection")->AsScalar();
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion


#pragma region BuildShadowMapEffect
BuildShadowMapEffect::BuildShadowMapEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	BuildShadowMapTech = mFX->GetTechniqueByName("BuildShadowMapTech");

	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
}

BuildShadowMapEffect::~BuildShadowMapEffect()
{
}
#pragma endregion


#pragma region Effects

BasicEffect* Effects::BasicFX = 0;
BuildShadowMapEffect* Effects::BuildShadowMapFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	BasicFX = new BasicEffect(device, L"./shaders/Basic.fx");
	BuildShadowMapFX = new BuildShadowMapEffect(device, L"./shaders/BuildShadowMap.fx");
}

void Effects::DestroyAll()
{
	SAFE_DELETE(BasicFX);
	SAFE_DELETE(BuildShadowMapFX);
}
#pragma endregion
