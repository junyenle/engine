#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
	float3 normal : NORMAL0;
    float4 color : COLOR0;
	float2 text : TEXCOORD0;
};

struct VOut // PS In
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
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
    output.color = vIn.color;
	output.text = vIn.text;
	output.normal = normalize(mul(float4(vIn.normal, 0), c_modelToWorld).xyz);

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
	// will need these a few times
	float3 N = pIn.normal;
	float3 V = normalize(c_cameraPosition - pIn.worldpos);
	float4 texColor = DiffuseTexture.Sample(DefaultSampler, pIn.text) * pIn.color;
	
	// ambient
	float3 phong = c_ambient;
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		PointLightData pointLight = c_pointLight[i];
		if (pointLight.isEnabled)
		{
			float3 L = normalize(pointLight.position - pIn.worldpos);
			float3 R = reflect(-L, N);
			float NL = dot(N, L);
			float RV = dot(R, V);
			float distanceMultiplier = smoothstep(pointLight.innerRadius, pointLight.outerRadius, length(pointLight.position - pIn.worldpos));
			if (NL > 0)
			{
				float3 ZERO = { 0.0f, 0.0f, 0.0f };
				// diffuse
				phong += lerp(pointLight.diffuseColor, ZERO, distanceMultiplier) * NL;
				// specular
				phong += lerp(pointLight.specularColor * pow(max(0, RV), pointLight.specularPower), ZERO, distanceMultiplier);
			}
		}
	}
	return texColor * float4(phong, 0);
}
