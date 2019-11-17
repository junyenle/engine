// We want to use row major matrices
#pragma pack_matrix(row_major)
#define MAX_POINT_LIGHTS 8

SamplerState DefaultSampler : register(s0);
Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);

struct PointLightData {
	float3 diffuseColor;
	float specularPower;
	float3 specularColor;
	float innerRadius;
	float3 position;
	float outerRadius;
	bool isEnabled;
	float3 padding;
};

cbuffer PerCameraConstants : register(b0)
{
    float4x4 c_viewProj;
    float3 c_cameraPosition;
};

cbuffer PerObjectConstants : register(b1)
{
    float4x4 c_modelToWorld;
};

cbuffer LightingConstants : register(b2)
{
	PointLightData c_pointLight[MAX_POINT_LIGHTS]; 
	float3 c_ambient;
	float padding;
};