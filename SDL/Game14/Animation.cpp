#include "Animation.h"
#include "Skeleton.h"
#include "LevelLoader.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>

bool Animation::Load(const std::string& inFileName) {
	fileName = inFileName;
	
	rapidjson::Document doc;
	if (!LevelLoader::LoadJSON(fileName, doc)) {
		SDL_Log("Failed to load animation %s", fileName.c_str());
		return false;
	}

	const auto& sequence = doc["sequence"];
	if (!sequence.IsObject()) {
		SDL_Log("Animation %s doesn't have a sequence.", fileName.c_str());
		return false;
	}

	const auto& frames = sequence["frames"];
	const auto& length = sequence["length"];
	const auto& bonecount = sequence["bonecount"];

	if (!frames.IsUint() || !length.IsFloat() || !bonecount.IsUint()) {
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", fileName.c_str());
		return false;
	}

	numBones = bonecount.GetUint();
	numFrames = frames.GetUint();
	duration = length.GetFloat();
	frameDuration = duration / (numFrames - 1);

	tracks.resize(numBones);

	const auto& jsonTracks = sequence["tracks"];
	if (!jsonTracks.IsArray()) {
		SDL_Log("Sequence %s missing a tracks array.", fileName.c_str());
		return false;
	}

	for (size_t i = 0; i < jsonTracks.Size(); ++i) {
		if (!jsonTracks[i].IsObject()) {
			SDL_Log("Animation %s: Track element %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const auto& transforms = jsonTracks[i]["transforms"];
		if (!transforms.IsArray()) {
			SDL_Log("Animation %s: Track element %d is missing transforms.", fileName.c_str(), i);
			return false;
		}		

		if (transforms.Size() < numFrames) {
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", fileName.c_str(), i);
			return false;
		}

		const auto boneIndex = jsonTracks[i]["bone"].GetUint();
		BoneTransform temp;

		for (size_t j = 0; j < transforms.Size(); ++j) {
			const auto& rot = transforms[j]["rot"];
			const auto& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray()) {
				SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
				return false;
			}

			temp.rotation.x = rot[0].GetFloat();
			temp.rotation.y = rot[1].GetFloat();
			temp.rotation.z = rot[2].GetFloat();
			temp.rotation.w = rot[3].GetFloat();

			temp.translation.x = trans[0].GetFloat();
			temp.translation.y = trans[1].GetFloat();
			temp.translation.z = trans[2].GetFloat();

			tracks[boneIndex].emplace_back(temp);
		}
	}

	return true;
}

std::vector<Matrix4> Animation::GetGlobalPoseAtTime(const Skeleton* inSkeleton, const float inTime) const {
	std::vector<Matrix4> outPoses(numBones);

	const auto frame = static_cast<size_t>(inTime / frameDuration);
	const auto pct = inTime / frameDuration - frame;

	const auto GetGlobalPose = [this, frame, pct](auto bone) {
		if (tracks[bone].size() == 0)
			return Matrix4::Identity;

		BoneTransform interp;

		if (Math::NearZero(pct))
			interp = tracks[bone][frame];
		else if (Math::NearZero(pct - 1.0f))
			interp = tracks[bone][frame + 1];
		else
			interp = BoneTransform::Interpolate(tracks[bone][frame], tracks[bone][frame + 1], pct);

		return interp.ToMatrix();
	};

	outPoses[0] = GetGlobalPose(0);

	const auto& bones = inSkeleton->GetBones();
	for (size_t bone = 1; bone < numBones; ++bone)
		outPoses[bone] = GetGlobalPose(bone) * outPoses[bones[bone].parent];

	return outPoses;
}