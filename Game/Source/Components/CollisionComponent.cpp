#include "stdafx.h"
#include "CollisionComponent.h"
#include "jsonUtil.h"
#include "RenderObj.h"
#include "game.h"
#include "Physics.h"

CollisionComponent::CollisionComponent(RenderObj* thing) : Component(thing)
{
	mRenderObj = thing;
	thing->mGame->mPhysics->AddObj(this);
}


CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::LoadProperties(const rapidjson::Value& properties)
{
	Component::LoadProperties(properties);
	GetVectorFromJSON(properties, "min", mAABB.mMin);
	GetVectorFromJSON(properties, "max", mAABB.mMax);
}

Physics::AABB CollisionComponent::GetAABB() const
{
	Physics::AABB toRet = mAABB;
	float scale = mRenderObj->mObjectData.data.GetScale().x;
	toRet.Scale(scale);
	Vector3 translate = mRenderObj->mObjectData.data.GetTranslation();
	toRet.Translate(translate);

	return toRet;
}
