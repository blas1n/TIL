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
		
		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		owner->SetPosition(pos);
	}
}