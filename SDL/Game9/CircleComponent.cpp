#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner)
	:Component(owner), radius(0.0f) {}

float CircleComponent::GetRadius() const {
	return owner->GetScale() * radius;
}

const Vector3& CircleComponent::GetCenter() const {
	return owner->GetPosition();
}

bool Intersect(const CircleComponent& a, const CircleComponent& b) {
	const auto diff = a.GetCenter() - b.GetCenter();
	const auto distSq = diff.LengthSquared();

	const auto radiiSq = a.GetRadius() + b.GetRadius();

	return distSq <= radiiSq * radiiSq;
}