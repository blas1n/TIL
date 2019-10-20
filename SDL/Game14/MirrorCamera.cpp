#include "MirrorCamera.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "JsonHelper.h"

MirrorCamera::MirrorCamera(Actor* inOwner)
	: CameraComponent(inOwner),
	horzDist(150.f),
	vertDist(200.0f),
	targetDist(400.0f) {

	SnapToIdeal();
}

void MirrorCamera::Update(const float deltaTime) {
	CameraComponent::Update(deltaTime);
	SnapToIdeal();
}

void MirrorCamera::SnapToIdeal() {
	const auto idealPos = ComputeCameraPos();
	const auto target = owner->GetPosition() -
		owner->GetForward() * targetDist;

	const auto view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);
	owner->GetGame()->GetRenderer()->SetMirrorView(view);
}

void MirrorCamera::LoadProperties(const rapidjson::Value& inObj) {
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetFloat(inObj, "horzDist", horzDist);
	JsonHelper::GetFloat(inObj, "vertDist", vertDist);
	JsonHelper::GetFloat(inObj, "targetDist", targetDist);
}

void MirrorCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObj) const {

	CameraComponent::SaveProperties(alloc, inObj);

	JsonHelper::AddFloat(alloc, inObj, "horzDist", horzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", vertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", targetDist);
}

Vector3 MirrorCamera::ComputeCameraPos() const {
	auto pos = owner->GetPosition();
	pos += owner->GetForward() * horzDist;
	pos += Vector3::UnitZ * vertDist;
	return pos;
}