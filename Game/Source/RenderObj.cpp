#include "stdafx.h"
#include "game.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "RenderObj.h"
#include "mesh.h"
#include "Component.h"

RenderObj::RenderObj(Game* pGame, const Mesh* pMesh)
{
	mGame = pGame;
	mMesh = pMesh;
	mObjectData.data = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
	mObjectBuffer = mGame->GetGraphics()->CreateGraphicsBuffer(&(mObjectData.data), sizeof(mObjectData.data), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}


RenderObj::~RenderObj()
{
	mObjectBuffer->Release();
	for (Component* component : mComponents)
	{
		delete component;
	}
}

void RenderObj::Draw()
{
	mGame->GetGraphics()->UploadBuffer(mObjectBuffer, &(mObjectData.data), sizeof(mObjectData.data));
	mGame->GetGraphics()->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_RENDEROBJ, 1, &mObjectBuffer);
	mMesh->Draw();
}

std::vector<class Component*> mComponents;
void RenderObj::AddComponent(Component* component)
{
	mComponents.push_back(component);
}
void RenderObj::Update(float deltaTime)
{
	for (Component* component : mComponents)
	{
		component->Update(deltaTime);
	}
}