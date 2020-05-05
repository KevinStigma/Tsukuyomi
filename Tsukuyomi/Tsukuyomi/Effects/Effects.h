#ifndef EFFECTS_H
#define EFFECTS_H

#include "d3dx11effect.h"
#include "LightHelper.h"
#include <string>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma region Effect
using namespace DirectX;
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region BasicEffect
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const RenderLightHelper::DirLight* lights, int count=3)   { DirLights->SetRawValue(lights, 0, count*sizeof(RenderLightHelper::DirLight)); }
	void SetPointLights(const RenderLightHelper::PointLight* lights, int count=3) { PointLights->SetRawValue(lights, 0, count * sizeof(RenderLightHelper::PointLight)); }
	void SetMaterial(const RenderLightHelper::Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(RenderLightHelper::Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* rv)		{ DiffuseMap->SetResource(rv); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLightCount(int c) { curDirLightCount->SetInt(c); }
	void SetPointLightCount(int c) { curPointLightCount->SetInt(c); }

	ID3DX11EffectTechnique* DebugNormalTech;
	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;
	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique*	Light2TexTech;
	ID3DX11EffectTechnique*	Light3TexTech;
	ID3DX11EffectTechnique* SimpleColorTech;
	ID3DX11EffectTechnique* CustomLightTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* curPointLightCount;
	ID3DX11EffectScalarVariable* curDirLightCount;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();
	static BasicEffect* BasicFX;
};
#pragma endregion

#endif // EFFECTS_H