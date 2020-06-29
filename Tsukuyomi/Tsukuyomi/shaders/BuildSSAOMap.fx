cbuffer cbPerFrame
{
	float gFarPlaneDepth;
	float2 gFarPlaneSize;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gNormalDepthMap;
Texture2D gRandomVecMap;

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldView;
	float4x4 gWorldInvTransposeView;
	float4x4 gWorldInvTranspose;
	float4x4 gViewProj;
	float4x4 gViewToTexSpace; // Proj*Texture
	float4x4 gWorldViewProj;

	float4   gOffsetVectors[14];

	// Coordinates given in view space.
	float    gOcclusionRadius = 0.5f;
	float    gOcclusionFadeStart = 0.2f;
	float    gOcclusionFadeEnd = 2.0f;
	float    gSurfaceEpsilon = 0.05f;
}; 
 
SamplerState samNormalDepth
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	// Set a very far depth value if sampling outside of the NormalDepth map
	// so we do not get false occlusions.
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f);
};


SamplerState samRandomVec
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosV : POSITION;
	float3 NormalV : NORMAL;
};
 
VertexOut NormalDepthVS(VertexIn vin)
{
	VertexOut vout;

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.PosV = mul(float4(vin.PosL, 1.0f), gWorldView).xyz;
	vout.NormalV = mul(float4(vin.NormalL, 1.0f), gWorldInvTransposeView).xyz;

	return vout;
}

float4 NormalDepthPS(VertexOut pin) : SV_Target
{
	float3 n = normalize(pin.NormalV);
	return float4(1.0, 1.0, 1.0, pin.PosV.z);
}

struct InitalSSAOVertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD0;
};


// Determines how much the sample point q occludes the point p as a function
// of distZ.
float OcclusionFunction(float distZ)
{
	//
	// If depth(q) is "behind" depth(p), then q cannot occlude p.  Moreover, if 
	// depth(q) and depth(p) are sufficiently close, then we also assume q cannot
	// occlude p because q needs to be in front of p by Epsilon to occlude p.
	//
	// We use the following function to determine the occlusion.  
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//

	float occlusion = 0.0f;
	if (distZ > gSurfaceEpsilon)
	{
		float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;

		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
		occlusion = saturate((gOcclusionFadeEnd - distZ) / fadeLength);
	}

	return occlusion;
}

InitalSSAOVertexOut InitalSSAOVS(VertexIn vin)
{
	InitalSSAOVertexOut vout;

	vout.PosH = float4(vin.PosL.x * gFarPlaneSize.x, vin.PosL.y * gFarPlaneSize.y, gFarPlaneDepth, 1.0);
	vout.Tex = vin.Tex;
	return vout;
}

float4 InitalSSAOPS(InitalSSAOVertexOut pin) : SV_Target
{
	float4 normalDepth = gNormalDepthMap.SampleLevel(samNormalDepth, pin.Tex, 0.0f);
	float3 n = normalDepth.xyz;
	float pz = normalDepth.w;
	
	float3 p = pz / pin.PosH.z * pin.PosH.xyz;
	
	// Extract random vector and map from [0,1] --> [-1, +1].
	float3 randVec = 2.0f * gRandomVecMap.SampleLevel(samRandomVec, 4.0f*pin.Tex, 0.0f).rgb - 1.0f;
	float occlusionSum = 0.0f;

	int sampleCount = 14;
	[unroll]
	for (int i = 0; i < sampleCount; ++i)
	{
		float3 offset = reflect(gOffsetVectors[i].xyz, randVec);
		float flip = sign(dot(offset, n));
		float3 q = p + flip * gOcclusionRadius * offset;
		float4 projQ = mul(float4(q, 1.0f), gViewToTexSpace);
		projQ /= projQ.w;
		float rz = gNormalDepthMap.SampleLevel(samNormalDepth, projQ.xy, 0.0f).a;
		float3 r = (rz / q.z) * q;
		float dp = max(dot(n, normalize(r - p)), 0.0f);
		float occlusion = dp * OcclusionFunction(p.z - r.z);

		occlusionSum += occlusion;
	}

	occlusionSum /= sampleCount;

	float access = 1.0f - occlusionSum;

	// Sharpen the contrast of the SSAO map to make the SSAO affect more dramatic.
	return saturate(pow(access, 4.0f));
}

technique11 BuildNormalDepthMapTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, NormalDepthVS()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, NormalDepthPS()));
    }
}

technique11 BuildInitialSSAOMapTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, InitalSSAOVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, InitalSSAOPS()));
	}
}
