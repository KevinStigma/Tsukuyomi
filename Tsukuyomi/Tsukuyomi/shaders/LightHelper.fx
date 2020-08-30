#include "PBRHelper.fx"

#define PI 3.14159265

struct DirectionalLight
{
	float3 Color;
	float pad1;
	float3 Direction;
	float pad2;
};

struct PointLight
{ 
	float3 Color;
	float pad1;

	float3 Position;
	float Range;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float3 albedo;
	float  roughness;
	float  metallic;
};

float2 getSphericalMapTexCoord(float phi, float theta)
{
	return float2((theta + PI) / (2.0 * PI), (phi + PI * 0.5) / PI);
}

void ComputeDirectionalLight(Material mat, DirectionalLight light, float3 N, float3 V, float3 F0, out float3 lo)
{
	float3 L = normalize(-light.Direction);
	float3 H = normalize(V + L);
	float3 radiance = light.Color.rgb;

	// cook-torrance brdf
	float NDF = DistributionGGX(N, H, mat.roughness);
	float G = GeometrySmith(N, V, L, mat.roughness);
	float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= (1.0 - mat.metallic);

	float3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	float3 specular = numerator / max(denominator, 0.001);

	float NdotL = max(dot(N, L), 0.0);
	lo = (kD * mat.albedo / PI + specular) * radiance * NdotL;
}

void ComputePointLight(Material mat, PointLight light, float3 pos, float3 N, float3 V, float3 F0,
				   out float3 lo)
{
	float3 L = light.Position - pos;
	float distance = length(L);
	
	// Range test.
	if( distance > light.Range )
		return;
		
	// Normalize the light vector.
	L /= distance;
	float3 H = normalize(V + L);
	
	float attenuation = 1.0 / (distance * distance);
	float3 radiance = light.Color.rgb * attenuation;

	// cook-torrance brdf
	float NDF = DistributionGGX(N, H, mat.roughness);
	float G = GeometrySmith(N, V, L, mat.roughness);
	float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= (1.0 - mat.metallic);

	float3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	float3 specular = numerator / max(denominator, 0.001);

	float NdotL = max(dot(N, L), 0.0);
	lo = (kD * mat.albedo / PI + specular) * radiance * NdotL;
}


//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

//---------------------------------------------------------------------------------------
// Performs shadowmap test to determine if a pixel is in shadow.
//---------------------------------------------------------------------------------------

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow, 
                       Texture2D shadowMap, 
					   float4 shadowPosH)
{
	// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;
	
	// Depth in NDC space.
	float depth = shadowPosH.z;

	// Texel size.
	const float dx = SMAP_DX;

	float percentLit = 0.0f;
	const float2 offsets[9] = 
	{
		float2(-dx,  -dx), float2(0.0f,  -dx), float2(dx,  -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx,  +dx), float2(0.0f,  +dx), float2(dx,  +dx)
	};

	[unroll]
	for(int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow, 
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}