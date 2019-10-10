#include "Game.h"
#include <SDL/SDL_image.h>
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Game 3", 100, 100, 1024, 768, 0);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();
	ticksCount = SDL_GetTicks();
	isRunning = true;
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
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	if (updatingActors)
		pendingActors.emplace_back(actor);
	else
		actors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(pendingActors.begin(), pendingActors.end(), actor);
	if (iter != pendingActors.end()) {
		std::iter_swap(iter, pendingActors.end() - 1);
		pendingActors.pop_back();
	}

	iter = std::find(actors.begin(), actors.end(), actor);
	if (iter != actors.end()) {
		std::iter_swap(iter, actors.end() - 1);
		actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = sprites.begin();

	for (; iter != sprites.end(); ++iter)
		if (myDrawOrder < (*iter)->GetDrawOrder())
			break;

	sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	sprites.erase(std::find(sprites.begin(), sprites.end(), sprite));
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* texture = nullptr;
	auto iter = textures.find(fileName);

	if (iter != textures.end())
		texture = iter->second;
	else {
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		texture = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
		if (!texture) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		textures.emplace(fileName.c_str(), texture);
	}

	return texture;
}

void Game::AddAsteroid(Asteroid* ast) {
	asteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast) {
	auto iter = std::find(asteroids.begin(), asteroids.end(), ast);
	if (iter != asteroids.end())
		asteroids.erase(iter);
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

	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
		isRunning = false;

	updatingActors = true;

	for (auto actor : actors)
		actor->ProcessInput(state);

	updatingActors = false;
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = Math::Min((SDL_GetTicks() - ticksCount) / 1000.0f, 0.05f);
	ticksCount = SDL_GetTicks();

	updatingActors = true;
	for (auto actor : actors)
		actor->Update(deltaTime);
	updatingActors = false;

	for (auto pending : pendingActors)
		actors.emplace_back(pending);
	pendingActors.clear();

	for (auto actor : actors)
		if (actor->GetState() == Actor::State::EDead)
			delete actor;
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);

	for (auto sprite : sprites)
		sprite->Draw(renderer);

	SDL_RenderPresent(renderer);
}

void Game::LoadData() {
	ship = new Ship(this);
	ship->SetPosition(Vector2(512.0f, 384.0f));
	ship->SetRotation(Math::Pi / 2);

	constexpr int numAsteroids = 20;

	for (int i = 0; i < numAsteroids; ++i)
		new Asteroid(this);
}

void Game::UnloadData() {
	while (!actors.empty())
		delete actors.back();

	for (auto texture : textures)
		SDL_DestroyTexture(texture.second);

	textures.clear();
}