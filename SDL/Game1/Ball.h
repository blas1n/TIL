#pragma once

#include "Vector2.h"

class Ball {
public:
	Ball();

	void Update(float deltaTime, Vector2 p1Pos, Vector2 p2Pos);
	Vector2 GetPos() const;

private:
	Vector2 pos;
	Vector2 vel;
};

