#include "stdafx.h"
#include "SimpleRotate.h"
#include "jsonUtil.h"
#include "RenderObj.h"
#include "game.h"
#include "Physics.h"

SimpleRotate::SimpleRotate(RenderObj* thing) : Component(thing)
{
	mRenderObj = thing;
}


SimpleRotate::~SimpleRotate()
{
}

void SimpleRotate::LoadProperties(const rapidjson::Value& properties)
{
	Component::LoadProperties(properties);
	GetFloatFromJSON(properties, "speed", speed);
}

void SimpleRotate::Update(float deltaTime)
{
	mRenderObj->mObjectData.data = Matrix4::CreateRotationZ(speed * deltaTime) * mRenderObj->mObjectData.data;
}