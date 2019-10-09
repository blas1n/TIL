#pragma once

#include "Vector2.h"
#include <SDL/SDL.h>

class Paddle {
public:
	Paddle(const int inPlayerIndex);

	void ProcessInput(const Uint8* state);
	void Update(const float deltaTime);

	Vector2 GetPos() const;

private:
	Vector2 pos;
	int dir;

	const SDL_Scancode upKey;
	const SDL_Scancode downKey;
};

