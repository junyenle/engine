#pragma once
#include "BoneTransform.h"

class Animation
{
public:
	Animation(class Game* game);
	~Animation();
	uint32_t GetNumBones() const { return mNumBones; }
	uint32_t GetNumFrames() const { return mNumFrames; }
	float GetLength() const { return mLength; }
	static Animation* StaticLoad(const WCHAR* fileName, Game* pGame);
	bool Load(const WCHAR* fileName);
	Game* mGame;
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const;
private:
	uint32_t mNumBones;
	uint32_t mNumFrames;
	float mLength;
	std::vector<std::vector<BoneTransform>> mTracks;
};

