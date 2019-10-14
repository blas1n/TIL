#pragma once

#include "CameraComponent.h"

class OrbitCamera : public CameraComponent {
public:
	OrbitCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	inline float GetPitchSpeed() const { return pitchSpeed; }
	inline void SetPitchSpeed(float inPitchSpeed) { pitchSpeed = inPitchSpeed; }

	inline float GetYawSpeed() const { return yawSpeed; }
	inline void SetYawSpeed(float inYawSpeed) { yawSpeed = inYawSpeed; }

private:
	Vector3 offset;
	Vector3 up;
	float pitchSpeed;
	float yawSpeed;
};