#pragma once
#include "Physics.h"
#include "Component.h"
#include "RenderObj.h"
class CollisionComponent :
	public Component
{
public:
	CollisionComponent(RenderObj* thing);
	~CollisionComponent();
	void LoadProperties(const rapidjson::Value& properties);
	Physics::AABB GetAABB() const;
	RenderObj* mRenderObj;
private:
	Physics::AABB mAABB;
};

