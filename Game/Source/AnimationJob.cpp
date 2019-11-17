#include "stdafx.h"
#include "AnimationJob.h"
#include "engineMath.h"
#include "Components/Character.h"
#include "Skeleton.h"
#include "Animation.h"
#include "SkinnedObj.h"


AnimationJob::AnimationJob(Character* chara)
{
	character = chara;
}


AnimationJob::~AnimationJob()
{
}

void AnimationJob::DoIt()
{
	std::vector<Matrix4> combinedBindPoses;
	character->mCurrentAnim->GetGlobalPoseAtTime(combinedBindPoses, character->mSkeleton, character->mAnimationTime);
	for (int i = 0; i < character->mSkeleton->GetNumBones(); i++)
	{
		character->mSkinnedObj->mSkinConstants.skinMatrix[i] = character->mSkeleton->GetGlobalInvBindPoses()[i] * combinedBindPoses[i];
	}
}