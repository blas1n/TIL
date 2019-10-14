#include "OrbitCamera.h"
#include "Actor.h"

OrbitCamera::OrbitCamera(Actor* inOwner)
	: CameraComponent(inOwner),
	offset(-400.0f, 0.0f, 0.0f),
	up(Vector3::UnitZ),
	pitchSpeed(0.0f),
	yawSpeed(0.0f) {}

void OrbitCamera::Update(const float deltaTime) {
	CameraComponent::Update(deltaTime);

	const Quaternion yaw{ Vector3::UnitZ, yawSpeed * deltaTime };
	offset = Vector3::Transform(offset, yaw);
	up = Vector3::Transform(up, yaw);

	auto forward = -offset;
	forward.Normalized();
	
	auto right = Vector3::Cross(up, forward);
	right.Normalized();

	const Quaternion pitch{ right, pitchSpeed * deltaTime };
	offset = Vector3::Transform(offset, pitch);
	up = Vector3::Transform(up, pitch);

	const auto target = owner->GetPosition();
	const auto pos = target + offset;
	const auto view = Matrix4::CreateLookAt(pos, target, up);
	SetViewMatrix(view);
}