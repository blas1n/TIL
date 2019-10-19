#pragma once

#include <vector>
#include <string>
#include "BoneTransform.h"

class Animation {
public:
	bool Load(const std::string& fileName);

	std::vector<Matrix4> GetGlobalPoseAtTime(const class Skeleton* inSkeleton, float inTime) const;

	inline size_t GetNumBones() const { return numBones; }
	inline size_t GetNumFrames() const { return numFrames; }
	inline float GetDuration() const { return duration; }
	inline float GetFrameDuration() const { return frameDuration; }
	
private:
	size_t numBones;
	size_t numFrames;

	float duration;
	float frameDuration;
	
	std::vector<std::vector<BoneTransform>> tracks;
};