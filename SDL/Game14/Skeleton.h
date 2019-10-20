#pragma once

#include <string>
#include <vector>
#include "BoneTransform.h"

class Skeleton {
public:
	struct Bone {
		BoneTransform localBindPose;
		std::string name;
		int parent;
	};

	bool Load(const std::string& inFileName);

	inline const std::string& GetFileName() const { return fileName; }
	inline size_t GetNumBones() const { return bones.size(); }
	inline const Bone& GetBone(size_t idx) const { return bones[idx]; }
	inline const std::vector<Bone>& GetBones() const { return bones; }
	inline const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return globalInvBindPoses; }

protected:
	void ComputeGlobalInvBindPose();

private:
	std::vector<Bone> bones;
	std::vector<Matrix4> globalInvBindPoses;
	std::string fileName;
};