#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(Actor* owner, const int updateOrder)
	: MoveComponent(owner, updateOrder), nextNode(nullptr) {}

void NavComponent::Update(const float deltaTime) {
	if (nextNode) {
		const auto diff = owner->GetPosition() - nextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f)) {
			nextNode = nextNode->GetParent();
			TurnTo(nextNode->GetPosition());
		}
	}
	
	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start) {
	nextNode = start->GetParent();
	TurnTo(nextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos) {
	const auto dir = pos - owner->GetPosition();
	owner->SetRotation(Math::Atan2(-dir.y, dir.x));
}
