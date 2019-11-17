#include "stdafx.h"
#include "RenderCube.h"
#include "game.h"
#include "Shader.h"
#include "texture.h"
#include "VertexBuffer.h"
#include "mesh.h"

RenderCube::RenderCube(Game* pGame, const Shader* pShader, const Texture* pTex) : RenderObj(pGame, NULL)
{
	const VertexBuffer* vertexBuffer = new VertexBuffer(mGame->GetGraphics(), cubeVertex, sizeof(cubeVertex) / sizeof(BufferData::VertexPosNormalColorTex), sizeof(BufferData::VertexPosNormalColorTex), cubeIndex, sizeof(cubeIndex) / sizeof(uint16_t), sizeof(uint16_t));
	Mesh* mesh = new Mesh(pGame, vertexBuffer, pShader);
	mesh->SetTexture(Graphics::TEXTURE_SLOT_DIFFUSE, pTex);
	mMesh = mesh;
}


RenderCube::~RenderCube()
{
	delete mMesh;
}

BufferData::VertexPosNormalColorTex RenderCube::cubeVertex[] =
{
	// <0, 0, -1>
	{ Vector3(-0.5f, 0.5f, -0.5f), Vector3(0,0,-1), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1, 1) }, //0
	{ Vector3(0.5f, 0.5f, -0.5f), Vector3(0,0,-1), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) , Vector2(0, 1) },
	{ Vector3(0.5f, -0.5f, -0.5f), Vector3(0,0,-1), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) , Vector2(0, 0) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(0,0,-1), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) , Vector2(1, 0) },
	// <0, 1, 0>
	{ Vector3(0.5f, 0.5f, 0.5f), Vector3(0,1,0), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) , Vector2(0, 0) }, //4
	{ Vector3(-0.5f, 0.5f, 0.5f), Vector3(0,1,0), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) , Vector2(0, 1) },
	{ Vector3(0.5f, 0.5f, -0.5f), Vector3(0,1,0), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) , Vector2(1, 0) },
	{ Vector3(-0.5f, 0.5f, -0.5f), Vector3(0,1,0), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) , Vector2(1, 1) },
	// <1, 0, 0>
	{ Vector3(0.5f, -0.5f, 0.5f), Vector3(1,0,0), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) , Vector2(0, 0) }, //8
	{ Vector3(0.5f, 0.5f, 0.5f), Vector3(1,0,0), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) , Vector2(1, 0) },
	{ Vector3(0.5f, -0.5f, -0.5f), Vector3(1,0,0), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) , Vector2(0, 1) },
	{ Vector3(0.5f, 0.5f, -0.5f), Vector3(1,0,0), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) , Vector2(1, 1) },
	// 0, 0, 1>
	{ Vector3(0.5f, -0.5f, 0.5f), Vector3(0,0,1), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) , Vector2(0, 0) }, //12
	{ Vector3(0.5f, 0.5f, 0.5f), Vector3(0,0,1), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) , Vector2(1, 0) },
	{ Vector3(-0.5f, -0.5f, 0.5f), Vector3(0,0,1), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) , Vector2(0, 1) },
	{ Vector3(-0.5f, 0.5f, 0.5f), Vector3(0,0,1), Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) , Vector2(1, 1) },
	// <0, -1, 0>
	{ Vector3(0.5f, -0.5f, 0.5f), Vector3(0,-1,0), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f) , Vector2(0, 1) }, //16
	{ Vector3(-0.5f, -0.5f, 0.5f), Vector3(0,-1,0), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f) , Vector2(0, 0) },
	{ Vector3(0.5f, -0.5f, -0.5f), Vector3(0,-1,0), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f) , Vector2(1, 1) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(0,-1,0), Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f) , Vector2(1, 0) },
	// <-1, 0, 0>
	{ Vector3(-0.5f, -0.5f, 0.5f), Vector3(-1,0,0), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) , Vector2(0, 1) }, //20
	{ Vector3(-0.5f, 0.5f, 0.5f), Vector3(-1,0,0), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) , Vector2(0, 0) },
	{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(-1,0,0), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) , Vector2(1, 1) },
	{ Vector3(-0.5f, 0.5f, -0.5f), Vector3(-1,0,0), Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(1, 0) }
};

uint16_t RenderCube::cubeIndex[] =
{
	2, 1, 0,
	3, 2, 0,
	4, 5, 7,
	6, 4, 7,
	8, 9, 11,
	10, 8, 11,
	12, 14, 15,
	13, 12, 15,
	17, 16, 18,
	19, 17, 18,
	21, 20, 22,
	23, 21, 22
};