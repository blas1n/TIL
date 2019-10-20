#include "JsonHelper.h"

bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt) {
	const auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
		return false;

	const auto& property = iter->value;
	if (!property.IsInt())
		return false;

	outInt = property.GetInt();
	return true;
}

bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat) {
	const auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
		return false;

	const auto& property = iter->value;
	if (!property.IsFloat())
		return false;

	outFloat = property.GetFloat();
	return true;
}

bool JsonHelper::GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr) {
	const auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
		return false;

	const auto& property = iter->value;
	if (!property.IsString())
		return false;

	outStr = property.GetString();
	return true;
}

bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool) {
	const auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
		return false;

	const auto& property = iter->value;
	if (!property.IsBool())
		return false;

	outBool = property.GetBool();
	return true;
}

bool JsonHelper::GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector) {
	auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
		return false;

	auto& property = iter->value;
	if (!property.IsArray() || property.Size() != 3)
		return false;

	for (rapidjson::SizeType i = 0; i < 3; i++)
		if (!property[i].IsFloat())
			return false;

	outVector.x = property[0].GetFloat();
	outVector.y = property[1].GetFloat();
	outVector.z = property[2].GetFloat();

	return true;
}

bool JsonHelper::GetQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat) {
	auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
		return false;

	auto& property = iter->value;
	if (!property.IsArray() || property.Size() != 4)
		return false;

	for (rapidjson::SizeType i = 0; i < 4; i++)
		if (!property[i].IsFloat())
			return false;

	outQuat.x = property[0].GetFloat();
	outQuat.y = property[1].GetFloat();
	outQuat.z = property[2].GetFloat();
	outQuat.w = property[3].GetFloat();

	return true;
}

void JsonHelper::AddInt(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const int value) {

	rapidjson::Value v{ value };
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddFloat(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const float value) {

	rapidjson::Value v{ value };
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddString(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const std::string& value) {

	rapidjson::Value v;
	v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddBool(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const bool value) {

	rapidjson::Value v{ value };
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddVector3(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const Vector3& value) {

	rapidjson::Value v{ rapidjson::kArrayType };

	v.PushBack(rapidjson::Value{ value.x }.Move(), alloc);
	v.PushBack(rapidjson::Value{ value.y }.Move(), alloc);
	v.PushBack(rapidjson::Value{ value.z }.Move(), alloc);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddQuaternion(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const Quaternion& value) {

	rapidjson::Value v{ rapidjson::kArrayType };

	v.PushBack(rapidjson::Value{ value.x }.Move(), alloc);
	v.PushBack(rapidjson::Value{ value.y }.Move(), alloc);
	v.PushBack(rapidjson::Value{ value.z }.Move(), alloc);
	v.PushBack(rapidjson::Value{ value.w }.Move(), alloc);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}