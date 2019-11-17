#pragma once
#include "component.h"
#include "engineMath.h"

class FollowCam : public Component
{
public:
    FollowCam(RenderObj *pObj);

    void LoadProperties(const rapidjson::Value& properties) override;

    void Update(float deltaTime) override;

private:
    Vector3 mOffset;
};