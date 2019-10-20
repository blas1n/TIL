#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "MatrixPalette.h"
#include "LevelLoader.h"

bool Skeleton::Load(const std::string& inFileName) {
	fileName = inFileName;
	
	rapidjson::Document doc;
	if (!LevelLoader::LoadJSON(fileName, doc)) {
		SDL_Log("Failed to load skeleton %s", fileName.c_str());
		return false;
	}

	const auto& boneCount = doc["bonecount"];
	if (!boneCount.IsUint()) {
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
		return false;
	}

	const auto count = boneCount.GetUint();
	if (count > MAX_SKELETON_BONES) {
		SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
		return false;
	}

	bones.reserve(count);

	const auto& jsonBones = doc["bones"];
	if (!jsonBones.IsArray()) {
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
		return false;
	}
	if (jsonBones.Size() != count) {
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName.c_str());
		return false;
	}

	Bone temp;

	for (size_t i = 0; i < count; ++i) {
		if (!jsonBones[i].IsObject()) {
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		temp.name = jsonBones[i]["name"].GetString();
		temp.parent = jsonBones[i]["parent"].GetInt();
		
		const auto& bindpose = jsonBones[i]["bindpose"];
		if (!bindpose.IsObject()) {
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const auto& rot = bindpose["rot"];
		const auto& trans = bindpose["trans"];

		if (!rot.IsArray() || !trans.IsArray()) {
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		temp.localBindPose.rotation.x = rot[0].GetFloat();
		temp.localBindPose.rotation.y = rot[1].GetFloat();
		temp.localBindPose.rotation.z = rot[2].GetFloat();
		temp.localBindPose.rotation.w = rot[3].GetFloat();

		temp.localBindPose.translation.x = trans[0].GetFloat();
		temp.localBindPose.translation.y = trans[1].GetFloat();
		temp.localBindPose.translation.z = trans[2].GetFloat();

		bones.emplace_back(temp);
	}

	ComputeGlobalInvBindPose();
	return true;
}

void Skeleton::ComputeGlobalInvBindPose() {
	globalInvBindPoses.resize(GetNumBones());

	globalInvBindPoses[0] = bones[0].localBindPose.ToMatrix();
	for (size_t i = 1; i < globalInvBindPoses.size(); ++i) {
		const auto localMat = bones[i].localBindPose.ToMatrix();
		globalInvBindPoses[i] = localMat * globalInvBindPoses[bones[i].parent];
	}

	for (auto& globalInvBindPose : globalInvBindPoses)
		globalInvBindPose.Invert();
}