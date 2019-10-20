#pragma once

#include "CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(Actor* inOwner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	inline void SetHorizontalDistance(const float dist) { horzDist = dist; }
	inline void SetVerticalDistance(const float dist) { vertDist = dist; }
	inline void SetTargetDistance(const float dist) { targetDist = dist; }
	inline void SetSpringConstant(const float dist) { springConstant = dist; }

	TypeId GetType() const override { return TypeId::TFollowCamera; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	Vector3 ComputeCameraPos() const;

	Vector3 actualPos;
	Vector3 velocity;

	float horzDist;
	float vertDist;
	float targetDist;
	float springConstant;
};