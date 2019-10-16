#pragma once

#include "Actor.h"
#include "SoundEvent.h"
#include "Math.h"

class FPSActor : public Actor {
public:
	FPSActor(Game* inGame);

	void ActorInput(const InputState& inputState) override;
	void UpdateActor(float deltaTime) override;

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

private:
	void Shoot();
	void FixCollisions();

private:
	class FPSCamera* camera;
	class BoxComponent* box;
	class MeshComponent* mesh;
	class MoveComponent* move;
	class AudioComponent* audio;

	Vector2 speed;
	Actor* fpsModel;

	float attackDelay;
	float curAttackDelay;

	SoundEvent footstep;
	float lastFootstep;
};

