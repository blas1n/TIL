#pragma once

#include "Actor.h"

class Ship final : public Actor {
public:
	Ship(class Game* game);

private:
	void ActorInput(const struct InputState& state) override;
	void UpdateActor(float deltaTime) override;
	
private:
	class CircleComponent* circle;

	Vector2 velocityDir;
	Vector2 rotationDir;

	float speed;
	float laserCooldown;
};