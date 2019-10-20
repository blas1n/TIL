#include "Component.h"
#include "Actor.h"
#include "LevelLoader.h"
#include "JsonHelper.h"

std::map<Component::TypeId, const char*> Component::TypeNames{
	{ Component::TypeId::TComponent, "Component" },
	{ Component::TypeId::TAudioComponent, "AudioComponent" },
	{ Component::TypeId::TBoxComponent, "BoxComponent" },
	{ Component::TypeId::TCameraComponent, "CameraComponent" },
	{ Component::TypeId::TFollowCamera, "FollowCamera" },
	{ Component::TypeId::TMeshComponent, "MeshComponent" },
	{ Component::TypeId::TMoveComponent, "MoveComponent" },
	{ Component::TypeId::TSkeletalMeshComponent, "SkeletalMeshComponent" },
	{ Component::TypeId::TSpriteComponent, "SpriteComponent" },
	{ Component::TypeId::TMirrorCamera, "MirrorCamera" },
	{ Component::TypeId::TPointLightComponent, "PointLightComponent" },
	{ Component::TypeId::TTargetComponent, "TargetComponent" }
};

Component::Component(Actor* inOwner, int inUpdateOrder)
	:owner(inOwner), updateOrder(inUpdateOrder) {

	owner->AddComponent(this);
}

Component::~Component() {
	owner->RemoveComponent(this);
}

void Component::LoadProperties(const rapidjson::Value& inObj) {
	JsonHelper::GetInt(inObj, "updateOrder", updateOrder);
}

void Component::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
	JsonHelper::AddInt(alloc, inObj, "updateOrder", updateOrder);
}