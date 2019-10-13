#pragma once

#include "Actor.h"
#include "SoundEvent.h"

class CameraActor : public Actor {
public:
	CameraActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;
	void UpdateActor(float deltaTime) override;

	void SetFootstepSurface(float value);

private:
	class MoveComponent* move;
	class AudioComponent* audio;
	SoundEvent footstep;
	float lastFootstep;
};

