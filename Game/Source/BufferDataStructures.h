#pragma once
#include "engineMath.h"
#include "Graphics.h"

#define MAX_POINT_LIGHTS 8

namespace BufferData
{
	struct VertexPosNormalColorTex
	{
		Vector3 pos;
		Vector3 normal;
		Graphics::Color4 color;
		Vector2 uv;
	};

	struct VertexPosNormBoneWeightTex
	{
		Vector3 position;
		Vector3 normal;
		unsigned char boneIndices[4];
		unsigned char boneWeights[4];
		Vector2 text;
	};

	struct VertexPosNormalTex
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 uv;
	};
	struct VertexPosNormalTexTan
	{
		Vector3 pos;
		Vector3 normal;
		Vector3 tangent;
		Vector2 uv;
	};

	struct PointLightData {
		Vector3 diffuseColor;
		float specularPower;
		Vector3 specularColor;
		float innerRadius;
		Vector3 position;
		float outerRadius;
		bool isEnabled;
		Vector3 padding;
	};

	struct QuadVertexData
	{
		Vector3 position;
		Vector2 uv;
	};

	struct LightingData
	{
		PointLightData c_pointLight[MAX_POINT_LIGHTS];
		Vector3 c_ambient;
		float padding;
	};
}