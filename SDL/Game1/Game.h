#pragma once

#include <SDL/SDL.h>

struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Vector2 ballPos;
	Vector2 ballVel;
	Vector2 paddlePos;

	Uint32 ticksCount;
	Sint32 paddleDir;
	bool isRunning;
};

