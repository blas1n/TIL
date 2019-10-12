#pragma once

#include "MoveComponent.h"

class InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const std::uint8_t* keyState) override;
	
#pragma region GetterAndSetter
	inline float GetMaxForwardSpeed() const { return maxForwardSpeed; }
	inline void SetMaxForwardSpeed(const float inMaxForwardSpeed) { maxForwardSpeed = inMaxForwardSpeed; }

	inline float GetMaxAngularSpeed() const { return maxAngularSpeed; }
	inline void SetMaxAngularSpeed(const float inMaxAngularSpeed) { maxAngularSpeed = inMaxAngularSpeed; }

	inline int GetForwardKey() const { return forwardKey; }
	inline void SetForwardKey(const int inForwardKey) { forwardKey = inForwardKey; }

	inline int GetBackKey() const { return backKey; }
	inline void SetBackKey(const int inBackKey) { backKey = inBackKey; }

	inline int GetClockwiseKey() const { return clockwiseKey; }
	inline void SetClockwiseKey(const int inClockwiseKey) { clockwiseKey = inClockwiseKey; }

	inline int GetCounterClockwiseKey() const { return counterClockwiseKey; }
	inline void SetCounterClockwiseKey(const int inCounterClockwiseKey) { counterClockwiseKey = inCounterClockwiseKey; }
#pragma endregion
	
private:
	float maxForwardSpeed;
	float maxAngularSpeed;

	int forwardKey;
	int backKey;

	int clockwiseKey;
	int counterClockwiseKey;
};
