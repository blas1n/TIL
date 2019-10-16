#pragma once

#include "CameraComponent.h"

class FPSCamera : public CameraComponent {
public:
	FPSCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	inline float GetPitchSpeed() const { return pitchSpeed; }
	inline void SetPitchSpeed(float inPitchSpeed) { pitchSpeed = inPitchSpeed; }

	inline float GetMaxPitch() const { return maxPitch; }
	inline void SetMaxPitch(const float inMaxPitch) { maxPitch = inMaxPitch; }

	inline float GetPitch() const { return pitch; }

private:
	float pitchSpeed;
	float maxPitch;
	float pitch;
};