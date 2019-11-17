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
	float4 color = float4(DiffuseTexture.Sample(DefaultSampler, pIn.tex).xyz, 1.0f);
	return color;
}
