#include "Paddle.h"
#include "Constant.h"

Paddle::Paddle(const int inPlayerIndex)
	: pos({ inPlayerIndex == 0 ? 25.0f : SCREEN_W - 25.0f, SCREEN_H * 0.5f }),
	dir(0),
	upKey(inPlayerIndex == 0 ? SDL_SCANCODE_W : SDL_SCANCODE_UP),
	downKey(inPlayerIndex == 0 ? SDL_SCANCODE_S : SDL_SCANCODE_DOWN)
{}

void Paddle::ProcessInput(const Uint8* state) {
	dir = 0;

	if (state[upKey])
		--dir;
	if (state[downKey])
		++dir;
}

void Paddle::Update(const float deltaTime) {
	if (dir != 0) {
		pos.y += dir * 300.0f * deltaTime;

		const auto minH = PADDLE_H * 0.5f + THICKNESS;
		const auto maxH = SCREEN_H - PADDLE_H * 0.5f - THICKNESS;

		if (pos.y < minH)
			pos.y = minH;
		else if (pos.y > maxH)
			pos.y = maxH;
	}
}

Vector2 Paddle::GetPos() const {
	return pos;
}