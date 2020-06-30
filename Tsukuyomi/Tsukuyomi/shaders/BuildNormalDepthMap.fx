cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldView;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTransposeView;
}; 

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
	float3 PosV       : POSITION;
	float3 NormalV    : NORMAL;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosV = mul(float4(vin.PosL, 1.0f), gWorldView).xyz;
	vout.NormalV = mul(float4(vin.NormalL, 0.0f), gWorldInvTransposeView).xyz;
		
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	return vout;
}
 

float4 PS(VertexOut pin) : SV_Target
{
	float3 n = normalize(pin.NormalV);
	return float4(n, pin.PosV.z);
}

technique11 BuildNormalDepthMap
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
