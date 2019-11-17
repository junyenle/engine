#include "stdafx.h"
#include "game.h"
#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"

Skeleton::Skeleton(Game* game)
{
	mGame = game;
}


Skeleton::~Skeleton()
{
}

void Skeleton::ComputeGlobalInvBindPose()
{
	mGlobalInvBindPoses.clear();
	for (int i = 0; i < mBones.size(); i++)
	{
		Matrix4 bpMatrix; // to push into mGlobalInvBindPoses vector
		Matrix4 lbpMatrix = mBones[i].mLocalBindPose.ToMatrix();
		if (mBones[i].mParent + 1)
		{
			bpMatrix = lbpMatrix * mGlobalInvBindPoses[mBones[i].mParent];
		}
		mGlobalInvBindPoses.push_back(bpMatrix);
	}
	for (int i = 0; i < mGlobalInvBindPoses.size(); i++)
	{
		mGlobalInvBindPoses[i].Invert();
	}
}

Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, Game* pGame)
{
	Skeleton* skeleton = new Skeleton(pGame);
	if (false == skeleton->Load(fileName))
	{
		delete skeleton;
		return 0;
	}
	return skeleton;
}

bool Skeleton::Load(const WCHAR* fileName)
{
	std::ifstream ifile;
	ifile.open(fileName);
	if (!ifile)
	{
		return false;
	}

	std::stringstream ss;
	ss << ifile.rdbuf();
	std::string contents = ss.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpskel" ||
		ver != 1)
	{
		return false;
	}

	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		return false;
	}

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray() || bones.Size() != bonecount.GetUint())
	{
		return false;
	}

	mBones.clear();
	for (unsigned i = 0; i < bonecount.GetUint(); i++)
	{
		Bone thisBone;
		if (!bones[i].IsObject())
		{
			return false;
		}
		const rapidjson::Value& name = bones[i]["name"];
		const rapidjson::Value& parent = bones[i]["parent"];
		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
		{
			return false;
		}
		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];
		if (!name.IsString() || !parent.IsInt() || !rot.IsArray() || !trans.IsArray())
		{
			return false;
		}

		thisBone.mName = name.GetString();
		thisBone.mParent = parent.GetInt();
		thisBone.mLocalBindPose.mRotation.x = rot[0].GetDouble();
		thisBone.mLocalBindPose.mRotation.y = rot[1].GetDouble();
		thisBone.mLocalBindPose.mRotation.z = rot[2].GetDouble();
		thisBone.mLocalBindPose.mRotation.w = rot[3].GetDouble();
		thisBone.mLocalBindPose.mTranslation.x = trans[0].GetDouble();
		thisBone.mLocalBindPose.mTranslation.y = trans[1].GetDouble();
		thisBone.mLocalBindPose.mTranslation.z = trans[2].GetDouble();

		mBones.push_back(thisBone);
	}

	ComputeGlobalInvBindPose();
	return true;
}