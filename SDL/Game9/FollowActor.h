#pragma once

#include "Actor.h"

class FollowActor : public Actor {
public:
	FollowActor(Game* inGame);
	void ActorInput(const InputState& inputState) override;
	void SetVisible(bool visible);

private:
	class FollowCamera* camera;
	class MoveComponent* move;
	class MeshComponent* mesh;
};