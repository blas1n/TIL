#include "FollowCamera.h"
#include "Actor.h"

FollowCamera::FollowCamera(Actor* inOwner)
	: CameraComponent(inOwner),
	actualPos(),
	velocity(),
	horizontalDist(350.0f),
	verticalDist(150.0f),
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

Vector3 FollowCamera::ComputeCameraPos() const {
	auto pos = owner->GetPosition();
	pos -= owner->GetForward() * horizontalDist;
	pos += Vector3::UnitZ * verticalDist;
	return pos;
}