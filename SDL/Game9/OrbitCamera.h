#pragma once

#include "CameraComponent.h"

class OrbitCamera : public CameraComponent {
public:
	OrbitCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	float GetPitchSpeed() const { return pitchSpeed; }
	void SetPitchSpeed(float inPitchSpeed) { pitchSpeed = inPitchSpeed; }

	float GetYawSpeed() const { return yawSpeed; }
	void SetYawSpeed(float inYawSpeed) { yawSpeed = inYawSpeed; }

private:
	Vector3 offset;
	Vector3 up;
	float pitchSpeed;
	float yawSpeed;
};