#include "stdafx.h"
#include "Camera.h"
#include "Components/followCam.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"
#include "stringUtil.h"


FollowCam::FollowCam(RenderObj *pObj)
    : Component(pObj)
{
	Matrix4 mat = mObj->mObjectData.data;
	mat = Matrix4::CreateRotationX(Math::PiOver2) * mat;	//mrwTODO this 90 rotation accounts for the orientation of the model :(
}

void FollowCam::LoadProperties(const rapidjson::Value& properties)
{
    Component::LoadProperties(properties);
    GetVectorFromJSON(properties, "offset", mOffset);
}

void FollowCam::Update(float deltaTime)
{
    Camera* pCamera = mObj->mGame->mCamera;
    Matrix4 mat = Matrix4::CreateTranslation(mObj->mObjectData.data.GetTranslation() + mOffset);
    mat.Invert();
    pCamera->mView = mat;
}