#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 text : TEXCOORD0;
};

struct VOut // PS In
{
    float4 position : SV_POSITION;
	float2 text : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 worldpos : POSITION0;
};

VOut VS(VIn vIn)
{
    VOut output;

    output.position = mul(float4(vIn.position, 1.0), c_modelToWorld);
	output.worldpos = output.position.xyz;
	output.position = mul(output.position, c_viewProj);
	output.text = vIn.text;
	output.normal = normalize(mul(float4(vIn.normal, 0), c_modelToWorld).xyz);

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
	// will need these a few times
	float3 N = pIn.normal;
	float3 V = normalize(c_cameraPosition - pIn.worldpos);
	return DiffuseTexture.Sample(DefaultSampler, pIn.text);
}
