#pragma once
#include "BoneTransform.h"

class Skeleton
{
public:
	Skeleton(class Game*);
	~Skeleton();
	struct Bone
	{
		BoneTransform mLocalBindPose;
		std::string mName;
		int mParent;
	};

	size_t GetNumBones() const 
	{
		return mBones.size();
	}
	const Bone& GetBone(size_t idx) const
	{
		return mBones[idx];
	}
	const std::vector<Bone>& GetBones() const
	{
		return mBones;
	}
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const
	{
		return mGlobalInvBindPoses;
	}
	static Skeleton* StaticLoad(const WCHAR* fileName, Game* pGame);
	bool Load(const WCHAR* fileName);
	
	Game* mGame;
private:
	std::vector<Bone> mBones;
	std::vector<Matrix4> mGlobalInvBindPoses;
	void ComputeGlobalInvBindPose();
};

