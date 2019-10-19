#include "BallMove.h"
#include "Game.h"
#include "Actor.h"
#include "PhysWorld.h"
#include "BallActor.h"
#include "TargetActor.h"

BallMove::BallMove(Actor* inOwner)
	: MoveComponent(inOwner),player(nullptr) {}

void BallMove::Update(const float deltaTime) {
	constexpr auto SegmentLength = 30.0f;

	const auto start = owner->GetPosition();
	const auto dir = owner->GetForward();
	const auto end = start + dir * SegmentLength;

	LineSegment l{ start, end };

	const auto phys = owner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	if (phys->SegmentCast(l, info) && info.actor != player) {
		const auto newDir = Vector3::Reflect(dir, info.normal);
		owner->RotateToNewForward(newDir);

		if (dynamic_cast<TargetActor*>(info.actor))
			static_cast<BallActor*>(owner)->HitTarget();
	}

	MoveComponent::Update(deltaTime);
}