#include "stdafx.h"
#include "game.h"
#include "Animation.h"
#include <fstream>
#include <sstream>
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"
#include "Skeleton.h"
#include "BoneTransform.h"

Animation::Animation(Game* game)
{
	mGame = game;
}


Animation::~Animation()
{
}

Animation* Animation::StaticLoad(const WCHAR* fileName, Game* pGame)
{
	Animation* animation = new Animation(pGame);
	if (false == animation->Load(fileName))
	{
		delete animation;
		return 0;
	}
	return animation;
}

bool Animation::Load(const WCHAR* fileName)
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
		str != "itpanim" ||
		ver != 2)
	{
		return false;
	}

	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		return false;
	}

	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];
	const rapidjson::Value& tracks = sequence["tracks"];
	if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint() || !tracks.IsArray())
	{
		return false;
	}

	this->mNumFrames = frames.GetUint();
	this->mLength = length.GetDouble();
	this->mNumBones = bonecount.GetUint();

	this->mTracks.resize(this->mNumBones);
	for (unsigned i = 0; i < tracks.Size(); i++)
	{
		if (!tracks[i].IsObject())
		{
			return false;
		}
		const rapidjson::Value& bone = tracks[i]["bone"];
		if (!bone.IsUint())
		{
			return false;
		}
		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray() || transforms.Size() != mNumFrames)
		{
			return false;
		}
		std::vector<BoneTransform> boneVec;
		mTracks.push_back(boneVec);
		for (unsigned j = 0; j < mNumFrames; j++)
		{
			BoneTransform boneTransform;
			if (!transforms[j].IsObject())
			{
				return false;
			}
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];
			if (!rot.IsArray() || !trans.IsArray())
			{
				return false;
			}
			boneTransform.mRotation.x = rot[0].GetDouble();
			boneTransform.mRotation.y = rot[1].GetDouble();
			boneTransform.mRotation.z = rot[2].GetDouble();
			boneTransform.mRotation.w = rot[3].GetDouble();
			boneTransform.mTranslation.x = trans[0].GetDouble();
			boneTransform.mTranslation.y = trans[1].GetDouble();
			boneTransform.mTranslation.z = trans[2].GetDouble();

			boneVec.push_back(boneTransform);
		}
		mTracks[bone.GetUint()] = boneVec;

	}

	return true;

}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	float timePerFrame = (GetLength() / (GetNumFrames() - 1));
	int firstKeyFrame = inTime / GetLength() * GetNumFrames();
	int secondKeyFrame = firstKeyFrame + 1 == GetNumFrames() ? 0 : firstKeyFrame + 1;
	float howFarPast = inTime - firstKeyFrame * timePerFrame;
	float howFarIn = howFarPast / timePerFrame;
	outPoses.clear();
	for (int i = 0; i < inSkeleton->GetNumBones(); i++)
	{
		Skeleton::Bone bone = inSkeleton->GetBone(i);
		Matrix4 combinedTransform = Matrix4();
		if (!mTracks[i].empty())
		{
			BoneTransform interpolatedBT = BoneTransform::Interpolate(mTracks[i][firstKeyFrame], mTracks[i][secondKeyFrame], howFarIn);
			//BoneTransform interpolatedBT = mTracks[i][firstKeyFrame]; // non interpolated
			combinedTransform *= interpolatedBT.ToMatrix();
			if (bone.mParent + 1)
			{
				combinedTransform *= outPoses[bone.mParent];
			}
		}
		outPoses.push_back(combinedTransform);
	}
}
