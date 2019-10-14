#include "SplineCamera.h"

Vector3 Spline::Compute(const size_t startIdx, const float t) const {
	const auto size = controlPoints.size();

	if (startIdx >= size)
		return controlPoints.back();
	if (startIdx == 0 || startIdx + 2 >= size)
		return controlPoints[startIdx];

	const auto p0 = controlPoints[startIdx - 1];
	const auto p1 = controlPoints[startIdx];
	const auto p2 = controlPoints[startIdx + 1];
	const auto p3 = controlPoints[startIdx + 2];

	return  0.5f * ((2.0f * p1) + (-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
}

SplineCamera::SplineCamera(Actor* inOwner)
	: CameraComponent(inOwner),
	spline(),
	index(1),
	t(0.0f),
	speed(0.5f),
	paused(true) {}

void SplineCamera::Update(const float deltaTime) {
	CameraComponent::Update(deltaTime);

	if (paused) return;

	t += speed * deltaTime;
	if (t >= 1.0f) {
		if (index < spline.GetNumPoints() - 3) {
			++index;
			t -= 1.0f;
		}
		else {
			paused = true;
		}
	}

	const auto pos = spline.Compute(index, t);
	const auto target = spline.Compute(index, t + 0.01f);
	SetViewMatrix(Matrix4::CreateLookAt(pos, target, Vector3::UnitZ));
}

void SplineCamera::Restart() {
	index = 1;
	t = 0.0f;
	paused = false;
}