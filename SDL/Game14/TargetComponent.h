#pragma once

#include "Component.h"

class TargetComponent :	public Component {
public:
	TargetComponent(Actor* inOwner);
	~TargetComponent();
	TypeId GetType() const override { return TypeId::TTargetComponent; }
};