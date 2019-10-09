#include "Ball.h"
#include "Constant.h"
#include <cmath>
#include <random>

Ball::Ball()
	: pos({ SCREEN_W * 0.5f, SCREEN_H * 0.5f }),
	vel({ 0.0f, 0.0f }) {

	std::random_device rd;
	std::mt19937_64 rnd(rd());

	std::uniform_real_distribution<float> range(180, 230);
	std::uniform_int_distribution<int> signRange(0, 1);

	vel.x = range(rnd) * (signRange(rnd) * 2 - 1);
	vel.y = range(rnd) * (signRange(rnd) * 2 - 1);
}

void Ball::Update(float deltaTime, Vector2 p1Pos, Vector2 p2Pos) {
	pos.x += vel.x * deltaTime;
	pos.y += vel.y * deltaTime;

	const auto isNearP1X = pos.x <= p1Pos.x && pos.x >= p1Pos.x - 5.0f;
	const auto isNearP2X = pos.x >= p2Pos.x && pos.x <= p2Pos.x + 5.0f;

	const auto isNearP1Y = fabsf(p1Pos.y - pos.y) <= PADDLE_H * 0.5f;
	const auto isNearP2Y = fabsf(p2Pos.y - pos.y) <= PADDLE_H * 0.5f;

	if ((isNearP1X && vel.x < 0.0f && isNearP1Y) || (isNearP2X && vel.x > 0.0f && isNearP2Y))
		 vel.x *= -1;

	if ((pos.y <= THICKNESS && vel.y < 0.0f)
		|| (pos.y >= (SCREEN_H - THICKNESS) && vel.y > 0.0f))
		vel.y *= -1;
}

Vector2 Ball::GetPos() const {
	return pos;
}