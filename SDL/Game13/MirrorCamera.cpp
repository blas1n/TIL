#include "MirrorCamera.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

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

Vector3 MirrorCamera::ComputeCameraPos() const {
	auto pos = owner->GetPosition();
	pos += owner->GetForward() * horzDist;
	pos += Vector3::UnitZ * vertDist;
	return pos;
}