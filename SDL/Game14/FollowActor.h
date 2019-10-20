#pragma once

#include "Actor.h"

class FollowActor : public Actor {
public:
	FollowActor(Game* inGame);
	void ActorInput(const InputState& inputState) override;
	void SetVisible(bool visible);

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	TypeId GetType() const override { return TypeId::TFollowActor; }

private:
	class MoveComponent* move;
	class FollowCamera* camera;
	class SkeletalMeshComponent* mesh;
	bool isMoving;
};