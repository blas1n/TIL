#pragma once

#include "CameraComponent.h"
#include <vector>

struct Spline {
	std::vector<Vector3> controlPoints;

	Vector3 Compute(size_t startIdx, float t) const;
	inline size_t GetNumPoints() const { return controlPoints.size(); }
};

class SplineCamera : public CameraComponent {
public:
	SplineCamera(Actor* inOwner);

	void Update(float deltaTime) override;
	void Restart();

	inline void SetSpline(const Spline& inSpline) { spline = inSpline; }
	inline void SetSpeed(const float inSpeed) { speed = inSpeed; }
	inline void SetPaused(const bool inPaused) { paused = inPaused; }
	
private:
	Spline spline;
	size_t index;

	float t;
	float speed;

	bool paused;
};