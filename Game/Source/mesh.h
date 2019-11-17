#pragma once

#include "engineMath.h"
#include "Graphics.h"

class Game;
class Shader;
class Texture;
class VertexBuffer;

class Mesh
{
public:
	Mesh(Game* pGame, const VertexBuffer* vertexBuffer, const Shader* pShader);
	~Mesh();

	void Draw() const;
	void SetTexture(int slot, const Texture* texture);

	bool Load(const WCHAR* fileName);
	static Mesh* StaticLoad(const WCHAR* fileName, Game* pGame);
	const Shader* mShader;
	const VertexBuffer* mVertexBuffer;
	const class Texture* mTextures[Graphics::TEXTURE_SLOT_TOTAL];
	Game* mGame;
	bool IsSkinned() const { return isSkinned; }
	bool isSkinned;
};