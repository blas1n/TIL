#include "Game.h"
#include "Constant.h"
	
Game::Game()
	: window(nullptr),
	renderer(nullptr),
	balls(BALL_COUNT),
	p1(0), p2(1),
	ticksCount(0),
	isRunning(true) {}

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
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	const auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
		isRunning = false;

	p1.ProcessInput(state);
	p2.ProcessInput(state);
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = fminf((SDL_GetTicks() - ticksCount) / 1000.0f, 0.05f);
	ticksCount = SDL_GetTicks();

	p1.Update(deltaTime);
	p2.Update(deltaTime);

	const auto p1Pos = p1.GetPos();
	const auto p2Pos = p2.GetPos();

	for (auto& ball : balls) {
		ball.Update(deltaTime, p1Pos, p2Pos);

		const auto ballX = ball.GetPos().x;
		if (ballX <= THICKNESS || ballX >= SCREEN_W - THICKNESS)
			isRunning = false;
	}
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect wall{ 0, 0, SCREEN_W, THICKNESS };
	SDL_RenderFillRect(renderer, &wall);

	wall.y = SCREEN_H - THICKNESS;
	SDL_RenderFillRect(renderer, &wall);

	SDL_Rect ballRect{ 0, 0, THICKNESS, THICKNESS };

	for (const auto& ball : balls) {
		auto pos = ball.GetPos();
		ballRect.x = static_cast<int>(pos.x - THICKNESS * 0.5f);
		ballRect.y = static_cast<int>(pos.y - THICKNESS * 0.5f);
		SDL_RenderFillRect(renderer, &ballRect);
	}

	auto paddlePos = p1.GetPos();
	SDL_Rect paddle{
		static_cast<int>(paddlePos.x - THICKNESS * 0.5f),
		static_cast<int>(paddlePos.y - PADDLE_H * 0.5f),
		THICKNESS,
		static_cast<int>(PADDLE_H)
	};
	
	SDL_RenderFillRect(renderer, &paddle);

	paddlePos = p2.GetPos();
	paddle.x = static_cast<int>(paddlePos.x - THICKNESS * 0.5f);
	paddle.y = static_cast<int>(paddlePos.y - PADDLE_H * 0.5f);

	SDL_RenderFillRect(renderer, &paddle);

	SDL_RenderPresent(renderer);
}