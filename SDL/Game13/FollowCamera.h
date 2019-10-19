#pragma once

#include "CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	inline void SetHorizontalDistance(const float dist) { horizontalDist = dist; }
	inline void SetVerticalDistance(const float dist) { verticalDist = dist; }
	inline void SetTargetDistance(const float dist) { targetDist = dist; }
	inline void SetSpringConstant(const float dist) { springConstant = dist; }

private:
	Vector3 ComputeCameraPos() const;

	Vector3 actualPos;
	Vector3 velocity;

	float horizontalDist;
	float verticalDist;
	float targetDist;
	float springConstant;
};