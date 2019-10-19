#pragma once

#include "CameraComponent.h"

class MirrorCamera : public CameraComponent {
public:
	MirrorCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	inline void SetHorzDist(float dist) { horzDist = dist; }
	inline void SetVertDist(float dist) { vertDist = dist; }
	inline void SetTargetDist(float dist) { targetDist = dist; }

private:
	Vector3 ComputeCameraPos() const;

	float horzDist;
	float vertDist;
	float targetDist;
};