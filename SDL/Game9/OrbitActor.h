#pragma once

#include "Actor.h"

class OrbitActor : public Actor {
public:
	OrbitActor(Game* inGame);

	void ActorInput(const InputState& inputState) override;

	void SetVisible(bool visible);

private:
	class OrbitCamera* camera;
	class MeshComponent* mesh;
};