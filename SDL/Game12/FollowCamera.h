#pragma once

#include "CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	inline void SetHorizontalDistance(const float inHorizontalDist) { horizontalDist = inHorizontalDist; }
	inline void SetVerticalDistance(const float inVerticalDist) { verticalDist = inVerticalDist; }
	inline void SetTargetDistance(const float inTargetDist) { targetDist = inTargetDist; }
	inline void SetSpringConstant(const float inSpringConstant) { springConstant = inSpringConstant; }

private:
	Vector3 ComputeCameraPos() const;

	Vector3 actualPos;
	Vector3 velocity;

	float horizontalDist;
	float verticalDist;
	float targetDist;
	float springConstant;
};