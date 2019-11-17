#include "Constants.hlsl"

struct VIn
{
	float3 position : POSITION0;
	float2 tex: TEXCOORD0;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 tex: TEXCOORD0;
};

VOut VS(VIn vIn)
{
	VOut output;

	output.position = float4(vIn.position, 1.0f);
	output.tex = vIn.tex;

	return output;
}

float4 PS(VOut pIn) : SV_Target
{
	float weight[] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
	float offset[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
	float2 horiOffset = float2(0.0, 4.0 / 600.0);
	float4 color = float4(DiffuseTexture.Sample(DefaultSampler, pIn.tex).xyz, 1.0f) * weight[0];
	for (int i = 1; i < 5; i++)
	{
		color += DiffuseTexture.Sample(DefaultSampler, pIn.tex + offset[i] * horiOffset) * weight[i];
	}
	return color;
}
