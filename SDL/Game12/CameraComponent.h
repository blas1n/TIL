#pragma once

#include "Component.h"
#include "Math.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* inOwner, int inUpdateOrder = 200);

protected:
	void SetViewMatrix(const Matrix4& view);
};