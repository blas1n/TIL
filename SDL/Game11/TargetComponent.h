#pragma once

#include "Component.h"

class TargetComponent :	public Component {
public:
	TargetComponent(Actor* inOwner);
	~TargetComponent();
};