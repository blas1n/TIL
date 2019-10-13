#pragma once

#include "Actor.h"

class CameraActor : public Actor {
public:
	CameraActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;
	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* move;
};

