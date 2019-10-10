#pragma once

#include "Actor.h"

class Asteroid final : public Actor {
public:
	Asteroid(class Game* game);
	~Asteroid();
	
	inline class CircleComponent* GetCircle() { return circle; }

private:
	void UpdateActor(float deltaTime) override;

private:
	class CircleComponent* circle;
};
