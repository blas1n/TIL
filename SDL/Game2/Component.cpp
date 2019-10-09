#include "Component.h"
#include "Actor.h"

Component::Component(Actor* inOwner, int inUpdateOrder)
	:owner(inOwner), updateOrder(inUpdateOrder) {

	owner->AddComponent(this);
}

Component::~Component() {
	owner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {}