#pragma once

#include "Component.h"
#include "Math.h"

class CircleComponent : public Component {
public:
	CircleComponent(class Actor* owner);
	
	inline float GetRadius() const { return owner->GetScale() * radius; }
	inline void SetRadius(float inRadius) { radius = inRadius; }
	
	inline const Vector2& GetCenter() const { return owner->GetPosition(); }

private:
	float radius;
};

bool Intersect(const CircleComponent& lhs, const CircleComponent& rhs);