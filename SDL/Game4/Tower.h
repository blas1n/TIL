#pragma once

#include "Actor.h"

class Tower : public Actor {
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* move;
	float nextAttack;
	constexpr static float AttackTime = 2.5f;
	constexpr static float AttackRange = 100.0f;
};
