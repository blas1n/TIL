#pragma once

#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;
	
#pragma region GetterAndSetter
	inline int GetForwardKey() const { return forwardKey; }
	inline void SetForwardKey(int inForwardKey) { forwardKey = inForwardKey; }

	inline int GetBackKey() const { return backKey; }
	inline void SetBackKey(int inBackKey) { backKey = inBackKey; }

	inline int GetClockwiseKey() const { return clockwiseKey; }
	inline void SetClockwiseKey(int inClockwiseKey) { clockwiseKey = inClockwiseKey; }

	inline int GetCounterClockwiseKey() const { return counterClockwiseKey; }
	inline void SetCounterClockwiseKey(int inCounterClockwiseKey) { counterClockwiseKey = inCounterClockwiseKey; }
#pragma endregion
	
private:
	int forwardKey;
	int backKey;

	int clockwiseKey;
	int counterClockwiseKey;
};
