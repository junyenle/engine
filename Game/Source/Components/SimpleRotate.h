#pragma once
#include "Physics.h"
#include "Component.h"
#include "RenderObj.h"
class SimpleRotate :
	public Component
{
public:
	SimpleRotate(RenderObj* thing);
	~SimpleRotate();
	void LoadProperties(const rapidjson::Value& properties);
	RenderObj* mRenderObj;
	void Update(float deltaTime) override;
	float speed;
};

