#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, const int updateOrder)
:Component(owner, updateOrder), forwardSpeed(0.0f), angularSpeed(0.0f) {}

void MoveComponent::Update(const float deltaTime) {
	if (!Math::NearZero(angularSpeed)) {
		float rot = owner->GetRotation();
		rot += angularSpeed * deltaTime;
		owner->SetRotation(rot);
	}
	
	if (!Math::NearZero(forwardSpeed)) {
		Vector2 pos = owner->GetPosition();
		pos += owner->GetForward() * forwardSpeed * deltaTime;
		owner->SetPosition(pos);
	}
}