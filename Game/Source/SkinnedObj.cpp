#include "stdafx.h"
#include "SkinnedObj.h"
#include "game.h"


SkinnedObj::SkinnedObj(Game* game, const Mesh* mesh) : RenderObj(game, mesh)
{
	for (int i = 0; i < MAX_SKELETON_BONES; i++)
	{
		mSkinConstants.skinMatrix[i] = Matrix4();
	}
	mBuffer = mGame->GetGraphics()->CreateGraphicsBuffer(&mSkinConstants, sizeof(mSkinConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}


SkinnedObj::~SkinnedObj()
{
	mBuffer->Release();
}

void SkinnedObj::Draw()
{
	mGame->GetGraphics()->UploadBuffer(mBuffer, &mSkinConstants, sizeof(mSkinConstants));
	mGame->GetGraphics()->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_SKINNING, 1, &mBuffer);
	RenderObj::Draw();
}