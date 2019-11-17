#pragma once
#include "engineMath.h"
#include "Graphics.h"
#include <vector>

class RenderObj
{
public:
	RenderObj(class Game* pGame, const class Mesh* pMesh);
	virtual void Draw();
	virtual ~RenderObj();
	struct PerObjectConstants
	{
		Matrix4 data;
	};
	struct PerObjectConstants mObjectData;
	const Mesh* mMesh;
	Game* mGame;
	ID3D11Buffer* mObjectBuffer;
	std::vector<class Component*> mComponents;
	void AddComponent(class Component* component);
	virtual void Update(float deltaTime);
};

