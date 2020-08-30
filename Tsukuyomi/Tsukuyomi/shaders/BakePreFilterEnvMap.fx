#include "LightHelper.fx"

cbuffer cbPerObject
{
	float gRoughness;
};

Texture2D gEnvironmentMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
	float2 Tex        : TEXCOORD;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	vout.PosH = float4(vin.PosL.x, vin.PosL.y, 1.0, 1.0f);

	// Pass onto pixel shader.
	vout.Tex = vin.Tex;
	return vout;
}

float VanDerCorpus(uint n, uint base)
{
	float invBase = 1.0 / float(base);
	float denom = 1.0;
	float result = 0.0;

	for (uint i = 0u; i < 32u; ++i)
	{
		if (n > 0u)
		{
			denom = fmod(float(n), 2.0);
			result += denom * invBase;
			invBase = invBase / 2.0;
			n = uint(float(n) / 2.0);
		}
	}

	return result;
}

float2 Hammersley(uint i, uint N)
{
	return float2(float(i) / float(N), VanDerCorpus(i, 2u));
}

float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
	float a = roughness * roughness;

	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	// from spherical coordinates to cartesian coordinates
	float3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	// from tangent-space vector to world-space sample vector
	float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
	float3 tangent = normalize(cross(up, N));
	float3 bitangent = normalize(cross(N, tangent));

	return normalize(tangent * H.x + bitangent * H.y + N * H.z);
}

float4 PS(VertexOut pin) : SV_Target
{
	float t = (pin.Tex.x - 0.5) * PI * 2.0;
	float p = (pin.Tex.y - 0.5) * PI;
	float3 N = normalize(float3(cos(p) * cos(t), sin(p), cos(p) * sin(t)));
	float3 R = N;
	float3 V = R;

	int SAMPLE_COUNT = 1024;
	float totalWeight = 0.0;
	float3 prefilteredColor = float3(0.0, 0.0, 0.0);
	float2 tex_coord;
	for (int i = 0; i < SAMPLE_COUNT; ++i)
	{
		float2 Xi = Hammersley(i, SAMPLE_COUNT);
		float3 H = ImportanceSampleGGX(Xi, N, gRoughness);
		float3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0);
		if (NdotL > 0.0)
		{
			tex_coord = getSphericalMapTexCoordFromVec(L);
			prefilteredColor += gEnvironmentMap.Sample(samLinear, tex_coord, 0.0f).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	prefilteredColor = prefilteredColor / totalWeight;

	return float4(prefilteredColor, 1.0);
}

technique11 BakePreFileterEnvMapTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
