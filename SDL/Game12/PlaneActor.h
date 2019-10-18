#pragma once

#include "Actor.h"

class PlaneActor : public Actor {
public:
	PlaneActor(class Game* inGame);
	~PlaneActor();
	inline class BoxComponent* GetBox() { return box; }

private:
	BoxComponent* box;
};

