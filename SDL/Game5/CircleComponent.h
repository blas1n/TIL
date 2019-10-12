#pragma once

#include "Component.h"
#include "Math.h"

class CircleComponent : public Component {
public:
	CircleComponent(class Actor* owner);
	
	float GetRadius() const;
	inline void SetRadius(float inRadius) { radius = inRadius; }
	
	const Vector2& GetCenter() const;

private:
	float radius;
};

bool Intersect(const CircleComponent& lhs, const CircleComponent& rhs);