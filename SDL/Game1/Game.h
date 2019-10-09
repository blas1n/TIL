#pragma once

#include <vector>
#include "Paddle.h"
#include "Ball.h"

class Game {
public:
	Game();

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

	std::vector<Ball> balls;

	Paddle p1;
	Paddle p2;

	Uint32 ticksCount;
	bool isRunning;
};

