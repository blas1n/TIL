#include "FollowCamera.h"
#include "Actor.h"
#include "JsonHelper.h"

FollowCamera::FollowCamera(Actor* inOwner)
	: CameraComponent(inOwner),
	actualPos(),
	velocity(),
	horzDist(350.0f),
	vertDist(250.0f),
	targetDist(100.0f),
	springConstant(64.0f) {}

void FollowCamera::Update(const float deltaTime) {
	CameraComponent::Update(deltaTime);

	const auto dampening = 2.0f * Math::Sqrt(springConstant);
	const auto idealPos = ComputeCameraPos();
	const auto diff = actualPos - idealPos;
	const auto acel = -springConstant * diff - dampening * velocity;

	velocity += acel * deltaTime;
	actualPos += velocity * deltaTime;

	const auto target = owner->GetPosition() + owner->GetForward() * targetDist;
	SetViewMatrix(Matrix4::CreateLookAt(actualPos, target, Vector3::UnitZ));
}

void FollowCamera::SnapToIdeal() {
	actualPos = ComputeCameraPos();
	velocity = Vector3::Zero;

	const auto target = owner->GetPosition() + owner->GetForward() * targetDist;
	SetViewMatrix(Matrix4::CreateLookAt(actualPos, target, Vector3::UnitZ));
}

void FollowCamera::LoadProperties(const rapidjson::Value& inObj) {
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "actualPos", actualPos);
	JsonHelper::GetVector3(inObj, "velocity", velocity);
	JsonHelper::GetFloat(inObj, "horzDist", horzDist);
	JsonHelper::GetFloat(inObj, "vertDist", vertDist);
	JsonHelper::GetFloat(inObj, "targetDist", targetDist);
	JsonHelper::GetFloat(inObj, "springConstant", springConstant);
}

void FollowCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObj) const {

	CameraComponent::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "actualPos", actualPos);
	JsonHelper::AddVector3(alloc, inObj, "velocity", velocity);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", horzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", vertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", targetDist);
	JsonHelper::AddFloat(alloc, inObj, "springConstant", springConstant);
}

Vector3 FollowCamera::ComputeCameraPos() const {
	auto pos = owner->GetPosition();
	pos -= owner->GetForward() * horzDist;
	pos += Vector3::UnitZ * vertDist;
	return pos;
}