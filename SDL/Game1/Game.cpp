#include "Game.h"
#include <SDL/SDL.h>

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	if (!(window = SDL_CreateWindow("Game1", 100, 100, 1024, 768, 0))) {
		SDL_Log("Failed to create window : %s", SDL_GetError());
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
		case SDL_QUIT: {
			isRunning = false;
			break;
		}
		}
	}

	const auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
		isRunning = false;
}

void Game::UpdateGame() {

}

void Game::GenerateOutput() {

}