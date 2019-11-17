#pragma once
#include "RenderObj.h"
#include "BufferDataStructures.h"

class RenderCube :
	public RenderObj
{
public:
	RenderCube(class Game* pGame, const class Shader* pShader, const class Texture* pTex);
	~RenderCube();
private:
	static BufferData::VertexPosNormalColorTex cubeVertex[24];
	static uint16_t cubeIndex[36];
};

