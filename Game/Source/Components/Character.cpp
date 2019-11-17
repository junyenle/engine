#include "stdafx.h"
#include "Character.h"
#include "../SkinnedObj.h"
#include "../RenderObj.h"
#include "../jsonUtil.h"
#include <string>
#include "../stringUtil.h"
#include "../game.h"
#include "../Animation.h"
#include "../JobManager.h"
#include "../AnimationJob.h"


Character::Character(SkinnedObj* thing) : Component(thing)
{
	mSkinnedObj = thing;
}


Character::~Character()
{
}

void Character::LoadProperties(const rapidjson::Value& properties)
{
	std::string skeletonFile;
	const rapidjson::Value& skeleton = properties["skeleton"];
	if (!skeleton.IsString()) { return; }
	skeletonFile = skeleton.GetString();

	std::wstring skeletonFileW;
	StringUtil::String2WString(skeletonFileW, skeletonFile);
	mSkeleton = mGame->LoadSkeleton(skeletonFileW);

	const rapidjson::Value& animations = properties["animations"];
	if (!animations.IsArray()) { return; }

	for (unsigned i = 0; i < animations.Size(); i++)
	{
		const rapidjson::Value& thisAnimation = animations[i];
		if (!thisAnimation.IsArray() || thisAnimation.Size() != 2)
		{
			return;
		}
		const rapidjson::Value& key = thisAnimation[0];
		const rapidjson::Value& filename = thisAnimation[1];
		if (!key.IsString() || !filename.IsString()) { return; }
		std::wstring animationFileW;
		StringUtil::String2WString(animationFileW, filename.GetString());
		mAnims[key.GetString()] = mGame->LoadAnimation(animationFileW);
	}
}

bool Character::SetAnim(const std::string& animName)
{
	std::unordered_map<std::string, const Animation*>::iterator animationIt = mAnims.find(animName);
	if (animationIt == mAnims.end()) { return false; }
	mCurrentAnim = animationIt->second;
	mAnimationTime = 0.0f;
	return true;
}

void Character::Update(float deltaTime)
{
	if (!mCurrentAnim)
	{
		SetAnim("idle");
	}
	UpdateAnim(deltaTime);
}

void Character::UpdateAnim(float deltaTime)
{
	mAnimationTime += deltaTime;
	while (mAnimationTime >= mCurrentAnim->GetLength())
	{
		mAnimationTime -= mCurrentAnim->GetLength();
	}

	AnimationJob* job = new AnimationJob(this);
	Game::jobManager->AddJob(job);
	//std::vector<Matrix4> combinedBindPoses;
	//mCurrentAnim->GetGlobalPoseAtTime(combinedBindPoses, mSkeleton, mAnimationTime);
	//for (int i = 0; i < mSkeleton->GetNumBones(); i++)
	//{
	//	mSkinnedObj->mSkinConstants.skinMatrix[i] = mSkeleton->GetGlobalInvBindPoses()[i] * combinedBindPoses[i];
	//}
}
