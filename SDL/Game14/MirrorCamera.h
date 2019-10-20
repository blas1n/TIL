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

	TypeId GetType() const override { return TypeId::TMirrorCamera; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	Vector3 ComputeCameraPos() const;

	float horzDist;
	float vertDist;
	float targetDist;
};