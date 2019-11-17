#pragma once
#include "Component.h"
#include <unordered_map>

class Character : public Component
{
	friend class AnimationJob;
public:
	Character(class SkinnedObj* thing);
	~Character();
	bool SetAnim(const std::string& animName);
	void LoadProperties(const rapidjson::Value& properties) override;
	void UpdateAnim(float deltaTime);
	void Update(float deltaTime) override;
	float mAnimationTime = 0.0f;
private:
	class SkinnedObj* mSkinnedObj;
	class Skeleton* mSkeleton;
	std::unordered_map<std::string, const class Animation*> mAnims;
	const Animation* mCurrentAnim;
};

