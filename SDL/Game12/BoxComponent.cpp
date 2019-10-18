#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

BoxComponent::BoxComponent(Actor* inOwner, int inUpdateOrder /*= 100*/)
	: Component(inOwner, inUpdateOrder),
	objectBox(Vector3::Zero, Vector3::Zero),
	worldBox(Vector3::Zero, Vector3::Zero),
	shouldRotate(true) {

	owner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent() {
	owner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform() {
	worldBox = objectBox;

	const auto scale = owner->GetScale();
	worldBox.min *= scale;
	worldBox.max *= scale;

	if (shouldRotate)
		worldBox.Rotate(owner->GetRotation());

	const auto position = owner->GetPosition();
	worldBox.min += position;
	worldBox.max += position;
}