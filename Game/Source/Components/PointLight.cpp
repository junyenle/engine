#include "stdafx.h"
#include "PointLight.h"
#include "RenderObj.h"
#include "jsonUtil.h"
#include "game.h"


PointLight::PointLight(RenderObj* pObj) : Component(pObj)
{
	mLight = mObj->mGame->AllocateLight();
	Matrix4 objTrans = mObj->mObjectData.data;
	mLight->position = Vector3(objTrans.mat[3][0], objTrans.mat[3][1], objTrans.mat[3][2]);
}


PointLight::~PointLight()
{
	mObj->mGame->FreeLight(mLight);
}


void PointLight::LoadProperties(const rapidjson::Value& properties) // pass this the light's properties
{
	Vector3 diffuseColor;
	Vector3 specularColor;
	float specularPower;
	float innerRadius;
	float outerRadius;

	GetVectorFromJSON(properties, "diffuseColor", diffuseColor);
	GetVectorFromJSON(properties, "specularColor", specularColor);
	GetFloatFromJSON(properties, "specularPower", specularPower);
	GetFloatFromJSON(properties, "innerRadius", innerRadius);
	GetFloatFromJSON(properties, "outerRadius", outerRadius);
	mLight->diffuseColor = diffuseColor;
	mLight->specularPower = specularPower;
	mLight->specularColor = specularColor;
	mLight->innerRadius = innerRadius;
	mLight->outerRadius = outerRadius;

}
