#pragma once
#include "Job.h"
class AnimationJob :
	public Job
{
public:
	AnimationJob(class Character* chara);
	~AnimationJob();
	void DoIt() override;
	Character* character;
};

