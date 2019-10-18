#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const {
	const auto rotMat = Matrix4::CreateFromQuaternion(rotation);
	const auto transMat = Matrix4::CreateTranslation(translation);
	return rotMat * transMat;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f) {
	BoneTransform retVal;
	retVal.rotation = Quaternion::Slerp(a.rotation, b.rotation, f);
	retVal.translation = Vector3::Lerp(a.translation, b.translation, f);
	return retVal;
}