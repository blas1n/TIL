#pragma once

#include "Actor.h"

class Ship final : public Actor {
public:
	Ship(class Game* game);

private:
	void ActorInput(const uint8_t* keyState) override;
	void UpdateActor(float deltaTime) override;
	
private:
	class CircleComponent* circle;

	float laserCooldown;
};