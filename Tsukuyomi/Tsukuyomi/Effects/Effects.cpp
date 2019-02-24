#include "Effects.h"
#include "../common.h"
#include <iostream>

#pragma region Effect
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	: mFX(0)
{
	auto hr=D3DX11CompileEffectFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, D3DCOMPILE_ENABLE_STRICTNESS, 0, device, &mFX, nullptr);
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

	Light1TexAlphaClipFogTech = mFX->GetTechniqueByName("Light1TexAlphaClipFog");

	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW           = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	DiffuseMap		  = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	TexTransform	  = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = mFX->GetVariableByName("gFogRange")->AsScalar();
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion

#pragma region Effects

BasicEffect* Effects::BasicFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	BasicFX = new BasicEffect(device, L"./shaders/Basic.fx");
}

void Effects::DestroyAll()
{
	SAFE_DELETE(BasicFX);
}
#pragma endregion
