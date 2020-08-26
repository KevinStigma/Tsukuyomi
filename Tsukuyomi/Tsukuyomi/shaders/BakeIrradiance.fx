#define PI 3.14159265

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

float4 PS(VertexOut pin) : SV_Target
{
	float3 color = gEnvironmentMap.Sample(samLinear, pin.Tex, 0.0f).rgb;

	return float4(color, 1.0);
}

technique11 BakeIrradianceTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
