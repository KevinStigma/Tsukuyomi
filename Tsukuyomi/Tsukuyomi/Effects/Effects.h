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
	void SetWorldViewProjTex(CXMMATRIX M)               { WorldViewProjTex->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform(CXMMATRIX M)				{ ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const RenderLightHelper::DirLight* lights, int count=3)   { DirLights->SetRawValue(lights, 0, count*sizeof(RenderLightHelper::DirLight)); }
	void SetPointLights(const RenderLightHelper::PointLight* lights, int count=3) { PointLights->SetRawValue(lights, 0, count * sizeof(RenderLightHelper::PointLight)); }
	void SetMaterial(const RenderLightHelper::Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(RenderLightHelper::Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* rv)		{ DiffuseMap->SetResource(rv); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)		{ ShadowMap->SetResource(tex); }
	void SetSSAOMap(ID3D11ShaderResourceView* sm)           { SSAOMap->SetResource(sm); }

	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLightCount(int c) { curDirLightCount->SetInt(c); }
	void SetPointLightCount(int c) { curPointLightCount->SetInt(c); }
	void SetGammaCorrect(int c) { gammaCorrect->SetInt(c); }
	void SetEnableHDR(int c) { enableHDR->SetInt(c); }
	void SetHDRExposure(float f) { HDRexposure->SetFloat(f); }

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
	ID3DX11EffectTechnique* CustomLightShadowTech;
	ID3DX11EffectTechnique* CustomLightShadowSSAOTech;
	ID3DX11EffectTechnique* CustomLightSSAOTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* WorldViewProjTex;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* curPointLightCount;
	ID3DX11EffectScalarVariable* curDirLightCount;
	ID3DX11EffectScalarVariable* gammaCorrect;
	ID3DX11EffectScalarVariable* HDRexposure;
	ID3DX11EffectScalarVariable* enableHDR;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* SSAOMap;
};
#pragma endregion


#pragma region DebugTexEffect
class DebugTexEffect : public Effect
{
public:
	DebugTexEffect(ID3D11Device* device, const std::wstring& filename);
	~DebugTexEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDebugTex(ID3D11ShaderResourceView* srv) { DebugTex->SetResource(srv); }

	
	ID3DX11EffectTechnique* DebugTexTech;
	ID3DX11EffectShaderResourceVariable* DebugTex;
	ID3DX11EffectMatrixVariable* WorldViewProj;
};
#pragma endregion

#pragma region BuildShadowMapEffect
class BuildShadowMapEffect : public Effect
{
public:
	BuildShadowMapEffect(ID3D11Device* device, const std::wstring& filename);
	~BuildShadowMapEffect();

	void SetViewProj(CXMMATRIX M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	ID3DX11EffectTechnique* BuildShadowMapTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
};
#pragma endregion


#pragma region BuildShadowMapEffect
class BuildNormalDepthMapEffect : public Effect
{
public:
	BuildNormalDepthMapEffect(ID3D11Device* device, const std::wstring& filename);
	~BuildNormalDepthMapEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldView(CXMMATRIX M) { WorldView->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTransposeView(CXMMATRIX M) { WorldInvTransposeView->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* BuildNormalDepthMapTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* WorldView;
	ID3DX11EffectMatrixVariable* WorldInvTransposeView;
};
#pragma endregion


#pragma region BuildSSAOMapEffect
class BuildSSAOMapEffect : public Effect
{
public:
	BuildSSAOMapEffect(ID3D11Device* device, const std::wstring& filename);
	~BuildSSAOMapEffect();

	void SetViewProj(CXMMATRIX M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewToTexSpace(CXMMATRIX M) { ViewToTexSpace->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetFarPlaneDepth(float depth) { FarPlaneDepth->SetFloat(depth); }
	void SetFarPlaneSize(CXMVECTOR v) { FarPlaneSize->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetOffsetVectors(const XMFLOAT4 v[14]) { OffsetVectors->SetFloatVectorArray(reinterpret_cast<const float*>(v), 0, 14); }
	void SetNormalDepthMap(ID3D11ShaderResourceView* srv) { NormalDepthMap->SetResource(srv); }
	void SetRandomVecMap(ID3D11ShaderResourceView* srv) { RandomVecMap->SetResource(srv); }

	ID3DX11EffectTechnique* BuildSSAOMapTech;

	ID3DX11EffectShaderResourceVariable* NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* RandomVecMap;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* ViewToTexSpace;
	ID3DX11EffectVectorVariable* OffsetVectors;
	ID3DX11EffectScalarVariable* FarPlaneDepth;
	ID3DX11EffectVectorVariable* FarPlaneSize;
};
#pragma endregion


#pragma region BlurSSAOEffect
class BlurSSAOEffect : public Effect
{
public:
	BlurSSAOEffect(ID3D11Device* device, const std::wstring& filename);
	~BlurSSAOEffect();

	void SetTexelWidth(float depth) { TexelWidth->SetFloat(depth); }
	void SetTexelHeight(float depth) { TexelHeight->SetFloat(depth); }
	void SetNormalDepthMap(ID3D11ShaderResourceView* srv) { NormalDepthMap->SetResource(srv); }
	void SetInputImage(ID3D11ShaderResourceView* srv) { InputImage->SetResource(srv); }

	ID3DX11EffectTechnique* HorizontalBlurTech;
	ID3DX11EffectTechnique* VerticalBlurTech;

	ID3DX11EffectShaderResourceVariable* NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* InputImage;

	ID3DX11EffectScalarVariable* TexelWidth;
	ID3DX11EffectScalarVariable* TexelHeight;
};
#pragma endregion


#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();
	static BasicEffect* BasicFX;
	static BuildNormalDepthMapEffect* BuildNormalDepthMapFX;
	static BuildShadowMapEffect* BuildShadowMapFX;
	static BuildSSAOMapEffect* BuildSSAOMapFX;
	static BlurSSAOEffect* SSAOBlurFX;
	static DebugTexEffect* DebugTexFX;
};
#pragma endregion

#endif // EFFECTS_H