#pragma once

#include "Actor.h"

class SplineActor : public Actor {
public:
	SplineActor(Game* inGame);

	void RestartSpline();

private:
	class SplineCamera* camera;
};