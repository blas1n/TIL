#include "Game.h"

constexpr Sint32 SCREEN_W = 1024;
constexpr Sint32 SCREEN_H = 768;

constexpr Sint32 THICKNESS = 15;
constexpr float PADDLE_H = 100.0f;
	
bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Game1", 50, 50, SCREEN_W, SCREEN_H, 0);
	if (!window) {
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}
	
	ballPos.x = SCREEN_W * 0.5f;
	ballPos.y = SCREEN_H * 0.5f;

	ballVel.x = -200.0f;
	ballVel.y = 235.0f;

	paddlePos.x = 10.0f;
	paddlePos.y = SCREEN_H * 0.5f;
	

	return true;
}

void Game::RunLoop() {
	while (isRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: {
			isRunning = false;
			break;
		}
		}
	}

	const auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
		isRunning = false;

	paddleDir = 0;
	if (state[SDL_SCANCODE_W])
		--paddleDir;
	if (state[SDL_SCANCODE_S])
		++paddleDir;
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = fminf((SDL_GetTicks() - ticksCount) / 1000.0f, 0.05f);
	ticksCount = SDL_GetTicks();

	if (paddleDir != 0) {
		paddlePos.y += paddleDir * 300.0f * deltaTime;

		const auto minH = PADDLE_H * 0.5f + THICKNESS;
		const auto maxH = SCREEN_H - PADDLE_H * 0.5f - THICKNESS;
		
		if (paddlePos.y < minH)
			paddlePos.y = minH;
		else if (paddlePos.y > maxH)
			paddlePos.y = maxH;
	}

	ballPos.x += ballVel.x * deltaTime;
	ballPos.y += ballVel.y * deltaTime;

	if (ballPos.x <= 0.0f)
		isRunning = false;

	const auto diff = fabsf(paddlePos.y - ballPos.y);
	if ((diff <= PADDLE_H * 0.5f && ballPos.x <= 25.0f && ballPos.x >= 20.0f && ballVel.x < 0.0f)
		|| (ballPos.x >= SCREEN_W - THICKNESS && ballVel.x > 0.0f))
		ballVel.x *= -1;

	if ((ballPos.y <= THICKNESS && ballVel.y < 0.0f)
		|| (ballPos.y >= (SCREEN_H - THICKNESS) && ballVel.y > 0.0f))
		ballVel.y *= -1;
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect wall{ 0, 0, SCREEN_W, THICKNESS };
	SDL_RenderFillRect(renderer, &wall);

	wall.y = SCREEN_H - THICKNESS;
	SDL_RenderFillRect(renderer, &wall);

	wall.x = SCREEN_W - THICKNESS;
	wall.y = THICKNESS;
	wall.w = THICKNESS;
	wall.h = static_cast<int>(SCREEN_H - THICKNESS * 2.0f);
	SDL_RenderFillRect(renderer, &wall);

	SDL_Rect ball{
		static_cast<int>(ballPos.x - THICKNESS / 2),
		static_cast<int>(ballPos.y - THICKNESS / 2),
		THICKNESS,
		THICKNESS
	};

	SDL_RenderFillRect(renderer, &ball);

	SDL_Rect paddle{
		static_cast<int>(paddlePos.x),
		static_cast<int>(paddlePos.y - PADDLE_H / 2),
		THICKNESS,
		static_cast<int>(PADDLE_H)
	};
	SDL_RenderFillRect(renderer, &paddle);

	SDL_RenderPresent(renderer);
}