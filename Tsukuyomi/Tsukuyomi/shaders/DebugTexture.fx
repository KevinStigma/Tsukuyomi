cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
}; 

Texture2D gDebugTex;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 1.0);
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
	float2 Tex  : TEXCOORD;
};
 
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.Tex  = vin.Tex;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float3 color = gDebugTex.SampleLevel(samLinear, pin.Tex.xy, 0.0f).rgb;
	return float4(color.x, color.x, color.x, 1.0);
}

technique11 DebugTexTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
