#pragma once

#include "Component.h"

class MoveComponent : public Component {
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;
	
	inline float GetForwardSpeed() const { return forwardSpeed; }
	inline void SetForwardSpeed(const float inForwardSpeed) { forwardSpeed = inForwardSpeed; }

	inline float GetStrafeSpeed() const { return strafeSpeed; }
	inline void SetStrafeSpeed(const float inStrafeSpeed) { strafeSpeed = inStrafeSpeed; }

	inline float GetAngularSpeed() const { return angularSpeed; }
	inline void SetAngularSpeed(const float inAngularSpeed) { angularSpeed = inAngularSpeed; }
	
	TypeId GetType() const override { return TypeId::TMoveComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	float forwardSpeed;
	float strafeSpeed;
	float angularSpeed;
};