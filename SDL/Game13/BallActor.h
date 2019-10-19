#pragma once

#include "Actor.h"

class BallActor : public Actor {
public:
	BallActor(Game* inGame);

	void UpdateActor(float deltaTime) override;

	void SetPlayer(Actor* player);
	void HitTarget();

private:
	class AudioComponent* audio;
	class BallMove* move;
	float lifeSpan;
};

