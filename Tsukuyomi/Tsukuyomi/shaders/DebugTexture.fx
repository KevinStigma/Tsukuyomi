cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
}; 

Texture2D gDebugTex;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
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
	//return float4(1.0, 0.0, 0.0, 1.0);
	float3 color = gDebugTex.Sample(samLinear, pin.Tex, 0.0f).rgb;
	return float4(color, 1.0);
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
