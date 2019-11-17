#include "stdafx.h"
#include "BoneTransform.h"


BoneTransform::BoneTransform()
{
}


BoneTransform::~BoneTransform()
{
}

Matrix4 BoneTransform::ToMatrix() const
{
	return Matrix4::CreateFromQuaternion(mRotation) * Matrix4::CreateTranslation(mTranslation);
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform toRet;
	toRet.mRotation = Lerp(a.mRotation, b.mRotation, f); //	NLerp is cheaper
	toRet.mRotation.Normalize();
	toRet.mTranslation = Lerp(a.mTranslation, b.mTranslation, f);
	return toRet;
}