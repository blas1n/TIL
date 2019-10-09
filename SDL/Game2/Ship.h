#pragma once

#include "Actor.h"

class Ship : public Actor {
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;

	void ProcessKeyboard(const uint8_t* state);

	inline float GetRightSpeed() const { return rightSpeed; }

	inline float GetDownSpeed() const { return downSpeed; }

private:
	float rightSpeed;

	float downSpeed;
};