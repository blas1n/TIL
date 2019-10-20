#pragma once

#include "Actor.h"

class PlaneActor : public Actor {
public:
	PlaneActor(Game* inGame);
	TypeId GetType() const override { return TypeId::TPlaneActor; }
};

