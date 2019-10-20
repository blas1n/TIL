#pragma once

#include "Actor.h"

class TargetActor : public Actor {
public:
	TargetActor(Game* inGame);
	TypeId GetType() const override { return TypeId::TTargetActor; }
};

