#pragma once
#include "engineMath.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/document.h"

bool GetFloatFromJSON(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
bool GetIntFromJSON(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
bool GetStringFromJSON(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
bool GetWStringFromJSON(const rapidjson::Value& inObject, const char* inProperty, std::wstring& outStr);
bool GetBoolFromJSON(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
bool GetVectorFromJSON(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
bool GetQuaternionFromJSON(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);
