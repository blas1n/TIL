#pragma once

#include "Math.h"

struct BoneTransform {
	Quaternion rotation;
	Vector3 translation;

	Matrix4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};