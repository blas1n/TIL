#pragma once

#include "Component.h"
#include "Math.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* inOwner, int inUpdateOrder = 200);

	TypeId GetType() const override { return TypeId::TCameraComponent; }

protected:
	void SetViewMatrix(const Matrix4& view);
};