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
	float t = (pin.Tex.x - 0.5) * PI * 2.0;
	float p = (pin.Tex.y - 0.5) * PI;
	float3 N = normalize(float3(cos(p) * cos(t), sin(p), cos(p) * sin(t)));
	float3 Nt, Nb;
	if (abs(N.x) > abs(N.y))
		Nt = float3(N.z, 0.0, -N.x);
	else
		Nt = float3(0.0, -N.z, N.y);
	Nt = normalize(Nt);
	Nb = normalize(cross(N, Nt));

	float sampleDelta = 0.025;
	int nSamples = 0;
	float3 irradiance = float3(0.0, 0.0, 0.0);
	for (float phi = 0; phi < PI * 2.0; phi += sampleDelta)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			float3 local_vec = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			float3 world_vec = Nt * local_vec.x + Nb * local_vec.y + N * local_vec.z;

			t = atan2(world_vec.z, world_vec.x);
			p = asin(world_vec.y);
			float2 tex_coord = float2((t + PI) / (2.0 * PI), (p + PI * 0.5) / PI);

			irradiance += gEnvironmentMap.Sample(samLinear, tex_coord, 0.0f).rgb * cos(theta) * sin(theta);
			nSamples++;
		}
	}
	irradiance *=  (1.0 / float(nSamples)) * PI;
	
	return float4(irradiance, 1.0);
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
