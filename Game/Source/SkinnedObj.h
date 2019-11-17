#pragma once
#include "RenderObj.h"
#define MAX_SKELETON_BONES 80

class SkinnedObj :
	public RenderObj
{
public:
	struct SkinConstants
	{
		Matrix4 skinMatrix[MAX_SKELETON_BONES];
	};
	SkinConstants mSkinConstants;
	ID3D11Buffer* mBuffer;
	SkinnedObj(Game* game, const Mesh* mesh);
	~SkinnedObj() override;
	void Draw() override;
};

