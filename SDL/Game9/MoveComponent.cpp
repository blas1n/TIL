#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, const int updateOrder)
	: Component(owner, updateOrder), forwardSpeed(0.0f), strafeSpeed(0.0f), angularSpeed(0.0f) {}

void MoveComponent::Update(const float deltaTime) {
	if (!Math::NearZero(angularSpeed)) {
		const auto rot = owner->GetRotation();
		const Quaternion inc{ Vector3::UnitZ, angularSpeed * deltaTime };
		owner->SetRotation(Quaternion::Concatenate(rot, inc));
	}
	
	if (!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed)) {
		auto pos = owner->GetPosition();
		pos += owner->GetForward() * forwardSpeed * deltaTime;
		pos += owner->GetRight() * strafeSpeed * deltaTime;
		owner->SetPosition(pos);
	}
}