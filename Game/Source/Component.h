#pragma once

#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"
class Component
{
public:
	Component(class RenderObj* pObj);
	virtual ~Component() {}
	virtual void LoadProperties(const rapidjson::Value& properties) {}
	virtual void Update(float deltatime) {}
	RenderObj* mObj;
	class Game* mGame;
};

