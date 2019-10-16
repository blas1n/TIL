#include "FPSCamera.h"
#include "Actor.h"

FPSCamera::FPSCamera(Actor* inOwner)
	: CameraComponent(inOwner),
	pitchSpeed(0.0f),
	maxPitch(Math::Pi / 3.0f),
	pitch(0.0f) {}

void FPSCamera::Update(const float deltaTime) {
	CameraComponent::Update(deltaTime);

	pitch += pitchSpeed * deltaTime;
	pitch = Math::Clamp(pitch, -maxPitch, maxPitch);

	Quaternion q{ owner->GetRight(), pitch };

	const auto pos = owner->GetPosition();
	const auto target = pos + Vector3::Transform(owner->GetForward(), q) * 100.0f;
	const auto up = Vector3::Transform(Vector3::UnitZ, q);

	SetViewMatrix(Matrix4::CreateLookAt(pos, target, up));
}