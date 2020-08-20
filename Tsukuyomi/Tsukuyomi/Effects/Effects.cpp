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
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		std::cout << "compile effect file failed!" << std::endl;
	}
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
	SimpleColorTech	  = mFX->GetTechniqueByName("SimpleColor");
	DebugNormalTech	  = mFX->GetTechniqueByName("DebugNormal");
	CustomLightTech   = mFX->GetTechniqueByName("CustomLight");
	CustomLightShadowTech = mFX->GetTechniqueByName("CustomLightShadow");
	CustomLightShadowSSAOTech = mFX->GetTechniqueByName("CustomLightShadowSSAO");
	CustomLightSSAOTech = mFX->GetTechniqueByName("CustomLightSSAO");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	WorldViewProjTex  = mFX->GetVariableByName("gWorldViewProjTex")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights         = mFX->GetVariableByName("gDirLights");
	PointLights		  = mFX->GetVariableByName("gPointLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap		  = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	ShadowMap		  = mFX->GetVariableByName("gShadowMap")->AsShaderResource();
	SSAOMap           = mFX->GetVariableByName("gSSAOMap")->AsShaderResource();
	ShadowTransform   = mFX->GetVariableByName("gShadowTransform")->AsMatrix();
	TexTransform	  = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
	curDirLightCount = mFX->GetVariableByName("curDirLightCount")->AsScalar();
	curPointLightCount = mFX->GetVariableByName("curPointLightCount")->AsScalar();
	gammaCorrect = mFX->GetVariableByName("gammaCorrection")->AsScalar();
	enableHDR = mFX->GetVariableByName("enableHDR")->AsScalar();
	HDRexposure = mFX->GetVariableByName("HDRexposure")->AsScalar();
	isLight = mFX->GetVariableByName("gIsLight")->AsScalar();
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion

#pragma region DebugTexEffect
DebugTexEffect::DebugTexEffect(ID3D11Device* device, const std::wstring& filename):Effect(device, filename)
{
	DebugTexTech = mFX->GetTechniqueByName("DebugTexTech");
	DebugTex = mFX->GetVariableByName("gDebugTex")->AsShaderResource();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

DebugTexEffect::~DebugTexEffect()
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

#pragma region BuildNormalDepthMapEffect
BuildNormalDepthMapEffect::BuildNormalDepthMapEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	BuildNormalDepthMapTech = mFX->GetTechniqueByName("BuildNormalDepthMap");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldView = mFX->GetVariableByName("gWorldView")->AsMatrix();
	WorldInvTransposeView = mFX->GetVariableByName("gWorldInvTransposeView")->AsMatrix();

	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
}

BuildNormalDepthMapEffect::~BuildNormalDepthMapEffect()
{

}
#pragma endregion

#pragma region BuildSSAOMapEffect
BuildSSAOMapEffect::BuildSSAOMapEffect(ID3D11Device* device, const std::wstring& filename) 
	:Effect(device, filename)
{
	BuildSSAOMapTech = mFX->GetTechniqueByName("BuildSSAOMapTech");

	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	ViewToTexSpace = mFX->GetVariableByName("gViewToTexSpace")->AsMatrix();
	FarPlaneSize = mFX->GetVariableByName("gFarPlaneSize")->AsVector();
	FarPlaneDepth = mFX->GetVariableByName("gFarPlaneDepth")->AsScalar();
	OffsetVectors = mFX->GetVariableByName("gOffsetVectors")->AsVector();

	NormalDepthMap = mFX->GetVariableByName("gNormalDepthMap")->AsShaderResource();
	RandomVecMap = mFX->GetVariableByName("gRandomVecMap")->AsShaderResource();
}

BuildSSAOMapEffect::~BuildSSAOMapEffect()
{
}
#pragma endregion

#pragma region BlurSSAOEffect
BlurSSAOEffect::BlurSSAOEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	HorizontalBlurTech = mFX->GetTechniqueByName("HorizontalBlurTech");
	VerticalBlurTech = mFX->GetTechniqueByName("VerticalBlurTech");

	TexelWidth = mFX->GetVariableByName("gTexelWidth")->AsScalar();
	TexelHeight = mFX->GetVariableByName("gTexelHeight")->AsScalar();

	NormalDepthMap = mFX->GetVariableByName("gNormalDepthMap")->AsShaderResource();
	InputImage = mFX->GetVariableByName("gInputImage")->AsShaderResource();
}

BlurSSAOEffect::~BlurSSAOEffect()
{
}
#pragma endregion

#pragma region EnvMapEffect
EnvMapEffect::EnvMapEffect(ID3D11Device* device, const std::wstring& filename) :Effect(device, filename)
{
	EnvMapTech = mFX->GetTechniqueByName("EnvMapTechnique");
	Mat = mFX->GetVariableByName("gMaterial");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EnvMap = mFX->GetVariableByName("gEnvironmentMap")->AsShaderResource();
}

EnvMapEffect::~EnvMapEffect()
{

}
#pragma endregion

#pragma region Effects

BasicEffect* Effects::BasicFX = 0;
BuildShadowMapEffect* Effects::BuildShadowMapFX = 0;
BuildSSAOMapEffect* Effects::BuildSSAOMapFX = 0;
BlurSSAOEffect* Effects::SSAOBlurFX = 0;
DebugTexEffect* Effects::DebugTexFX = 0;
BuildNormalDepthMapEffect* Effects::BuildNormalDepthMapFX = 0;
EnvMapEffect* Effects::EnvMapFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	BasicFX = new BasicEffect(device, L"./shaders/Basic.fx");
	BuildShadowMapFX = new BuildShadowMapEffect(device, L"./shaders/BuildShadowMap.fx");
	BuildSSAOMapFX = new BuildSSAOMapEffect(device, L"./shaders/BuildSSAOMap.fx");
	SSAOBlurFX = new BlurSSAOEffect(device, L"./shaders/SSAOBlur.fx");
	DebugTexFX = new DebugTexEffect(device, L"./shaders/DebugTexture.fx");
	BuildNormalDepthMapFX = new BuildNormalDepthMapEffect(device, L"./shaders/BuildNormalDepthMap.fx");
	EnvMapFX = new EnvMapEffect(device, L"./shaders/EnvironmentMap.fx");
}

void Effects::DestroyAll()
{
	SAFE_DELETE(BasicFX);
	SAFE_DELETE(BuildShadowMapFX);
	SAFE_DELETE(BuildSSAOMapFX);
	SAFE_DELETE(SSAOBlurFX);
	SAFE_DELETE(DebugTexFX);
	SAFE_DELETE(BuildNormalDepthMapFX);
	SAFE_DELETE(EnvMapFX);
}
#pragma endregion
