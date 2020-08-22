#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[10];
	PointLight gPointLights[10];
	float3 gEyePosW;
	int curPointLightCount;
	int curDirLightCount;
	int gammaCorrection;
	int enableHDR;

	float  HDRexposure;
	float  gFogStart;
	float  gFogRange;
	float4 gFogColor; 
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform; 
	float4x4 gShadowTransform;
	float4x4 gWorldViewProjTex;
	Material gMaterial;
	bool     gIsLight;
}; 

Texture2D gDiffuseMap;
Texture2D gShadowMap;
Texture2D gSSAOMap;
Texture2D gIrradianceMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	ComparisonFunc = LESS;
};

struct SimpleVertexIn
{
	float3 PosL    : POSITION;
	float4 Color   : COLOR;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
	float3 PosW       : POSITION;
	float3 NormalW    : NORMAL;
	float2 Tex        : TEXCOORD0;
	float4 ShadowPosH : TEXCOORD1;
	float4 SSAOPosH : TEXCOORD2;
	float4 Color      : COLOR;
};

VertexOut SimpleColorVS(SimpleVertexIn vin)
{
	VertexOut vout;

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.Color = vin.Color;

	return vout;
}

float4 SimpleColorPS(VertexOut vout) : SV_Target
{
	return vout.Color;
}

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	vout.ShadowPosH = mul(float4(vin.PosL, 1.0f), gShadowTransform);
	vout.SSAOPosH = mul(float4(vin.PosL, 1.0f), gWorldViewProjTex);
	return vout;
}
 
float4 NormalPS(VertexOut pin) : SV_Target
{
	pin.NormalW = normalize(pin.NormalW);
	return float4(pin.NormalW, 1.0);
}

float4 CustomPS(VertexOut pin,
	uniform int gPointLightCount,
	uniform int gDirLightCount,
	uniform bool gUseShadowMap,
	uniform bool gUseSSAO,
	uniform bool gUseTexure,
	uniform bool gAlphaClip,
	uniform bool gFogEnabled) : SV_Target
{

	if (gIsLight)
		return float4(gMaterial.albedo, 1.0);
	float3 color = float3(0.0f, 0.0f, 0.0f);

	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	float3 V = normalize(gEyePosW - pin.PosW);

	// Normalize.
	V = normalize(V);

	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, gMaterial.albedo, gMaterial.metallic);

	float3 L0 = float3(0.0, 0.0, 0.0);

	float shadow = 1.0;
	if (gUseShadowMap)
		shadow = CalcShadowFactor(samShadow, gShadowMap, pin.ShadowPosH);

	float ambient_weight = 1.0;
	if (gUseSSAO)
	{
		pin.SSAOPosH /= pin.SSAOPosH.w;
		ambient_weight = gSSAOMap.Sample(samLinear, pin.SSAOPosH.xy, 0.0f).r;
	}


	float theta = atan2(pin.NormalW.z, pin.NormalW.x);
	float phi = asin(pin.NormalW.y);

	float3 ks = fresnelSchlickRoughness(max(dot(pin.NormalW, V), 0.0), F0, gMaterial.roughness);
	float3 kd = (1.0 - ks) * (1.0 - gMaterial.metallic);
	float3 irradiance = gIrradianceMap.Sample(samLinear, float2((theta + PI) / (2.0 * PI), (phi + PI * 0.5) / PI), 0.0f).rgb;
	float3 ambient = irradiance * gMaterial.albedo * kd * ambient_weight;

	if(length(irradiance) < 1e-6)
		ambient = gMaterial.albedo * 0.03 * ambient_weight;
	
	//
	// Lighting.
	//

	if (gPointLightCount + gDirLightCount > 0)
	{
		// Sum the light contribution from each light source.  
		[unroll]
		for (int i = 0; i < gDirLightCount; ++i)
		{
			float3 lo = float3(0.0, 0.0, 0.0);
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, V, F0, lo);
			color += shadow * lo;
		}

		[unroll]
		for (int i = 0; i < gPointLightCount; i++)
		{
			float3 lo = float3(0.0, 0.0, 0.0);
			ComputePointLight(gMaterial, gPointLights[i], pin.PosW, pin.NormalW, V, F0, lo);
			color += shadow * lo;
		}
	}

	color += ambient;

	if (enableHDR)
	{
		float exposure = max(0.0, HDRexposure);
		color = 1.0 - exp(-color * exposure);
	}

	if (gammaCorrection)
	{
		float gamma_ratio = 1.0 / 2.2;
		color = pow(color, gamma_ratio);	
	}
	return float4(color, 1.0);
}

technique11 DebugNormal
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, NormalPS()));
	}
}

technique11 SimpleColor
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, SimpleColorVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, SimpleColorPS()));
	}
}

technique11 CustomLight
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, CustomPS(curPointLightCount, curDirLightCount, false, false, false, false, false)));
	}
}

technique11 CustomLightShadow
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, CustomPS(curPointLightCount, curDirLightCount, true, false, false, false, false)));
	}
}


technique11 CustomLightSSAO
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, CustomPS(curPointLightCount, curDirLightCount, false, true, false, false, false)));
	}
}


technique11 CustomLightShadowSSAO
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, CustomPS(curPointLightCount, curDirLightCount, true, true, false, false, false)));
	}
}
