#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "JsonHelper.h"

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

void BoxComponent::LoadProperties(const rapidjson::Value& inObj) {
	Component::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "objectMin", objectBox.min);
	JsonHelper::GetVector3(inObj, "objectMax", objectBox.max);
	JsonHelper::GetVector3(inObj, "worldMin", worldBox.min);
	JsonHelper::GetVector3(inObj, "worldMax", worldBox.max);
	JsonHelper::GetBool(inObj, "shouldRotate", shouldRotate);
}

void BoxComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObj) const {

	Component::SaveProperties(alloc, inObj);


	JsonHelper::AddVector3(alloc, inObj, "objectMin", objectBox.min);
	JsonHelper::AddVector3(alloc, inObj, "objectMax", objectBox.max);
	JsonHelper::AddVector3(alloc, inObj, "worldMin", worldBox.min);
	JsonHelper::AddVector3(alloc, inObj, "worldMax", worldBox.max);
	JsonHelper::AddBool(alloc, inObj, "shouldRotate", shouldRotate);
}