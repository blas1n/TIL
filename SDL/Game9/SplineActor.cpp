#include "SplineActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "SplineCamera.h"

SplineActor::SplineActor(Game* inGame)
	: Actor(inGame),
	camera(new SplineCamera(this)) {

	Spline path;
	path.controlPoints.emplace_back(Vector3::Zero);
	for (auto i = 0; i < 5; ++i) {
		if (i % 2 == 0)
			path.controlPoints.emplace_back(Vector3{ 300.0f * (i + 1), 300.0f, 300.0f });
		else
			path.controlPoints.emplace_back(Vector3{ 300.0f * (i + 1), 0.0f, 0.0f });
	}

	camera->SetSpline(path);
	camera->SetPaused(false);
}

void SplineActor::RestartSpline() {
	camera->Restart();
}