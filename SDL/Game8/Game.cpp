#include "Game.h"
#include <GL/glew.h>
#include <SDL/SDL_image.h>
#include <algorithm>
#include "InputSystem.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window = SDL_CreateWindow("Game 8", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	inputSystem = new InputSystem();
	if (!inputSystem->Initialize()) {
		SDL_Log("Failed to initialize input system");
		return false;
	}

	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW");
		return false;
	}

	glGetError();
	
	if (!LoadShaders()) {
		SDL_Log("Failed to load shader");
		return false;
	}

	CreateSpriteVerts();
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

	inputSystem->Shutdown();
	delete inputSystem;

	delete spriteVerts;
	spriteShader->Unload();
	delete spriteShader;
	SDL_GL_DeleteContext(context);
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

Texture* Game::GetTexture(const std::string& fileName) {
	Texture* texture = nullptr;
	auto iter = textures.find(fileName);

	if (iter != textures.end()) {
		texture = iter->second;
	}
	else {
		texture = new Texture();
		if (texture->Load(fileName)) {
			textures.emplace(fileName.c_str(), texture);
		}
		else {
			delete texture;
			texture = nullptr;
		}
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

void Game::DeadShip() {
	delete ship;
	ship = nullptr;

	goalBgColor = Random::GetVector(Vector3::Zero, Vector3::One);
	deadTimer = 2.0f;
}

void Game::ProcessInput() {
	inputSystem->PrepareForUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			inputSystem->ProcessEvent(event);
			break;
		}
	}

	inputSystem->Update();
	const auto state = inputSystem->GetState();

	if (state.keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::EReleased)
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

	curBgColor = Vector3::Lerp(curBgColor, goalBgColor, deltaTime);

	if (!ship) {
		if ((deadTimer -= deltaTime) <= 0.0f) {
			ship = new Ship(this);
			ship->SetRotation(Math::Pi / 2);
		}
	}

	updatingActors = true;
	for (auto actor : actors)
		actor->Update(deltaTime);
	updatingActors = false;

	for (auto pending : pendingActors)
		actors.emplace_back(pending);
	pendingActors.clear();

	std::vector<Actor*> deadActors;

	for (auto actor : actors)
		if (actor->GetState() == Actor::State::EDead)
			deadActors.emplace_back(actor);

	for (auto actor : deadActors)
		delete actor;
}

void Game::GenerateOutput() {
	glClearColor(curBgColor.x, curBgColor.y, curBgColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	spriteShader->SetActive();
	spriteVerts->SetActive();

	for (auto sprite : sprites)
		sprite->Draw(spriteShader);

	SDL_GL_SwapWindow(window);
}

bool Game::LoadShaders() {
	spriteShader = new Shader();
	if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
		return false;

	spriteShader->SetActive();
	const auto viewProj = Matrix4::CreateSimpleViewProj(1024.0f, 768.0f);
	spriteShader->SetMatrixUniform("uViewProjection", viewProj);
	return true;
}

void Game::CreateSpriteVerts() {
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	spriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData() {
	ship = new Ship(this);
	ship->SetRotation(Math::Pi / 2);

	constexpr int numAsteroids = 20;

	for (int i = 0; i < numAsteroids; ++i)
		new Asteroid(this);
}

void Game::UnloadData() {
	while (!actors.empty())
		delete actors.back();

	for (auto texture : textures) {
		texture.second->Unload();
		delete texture.second;
	}

	textures.clear();
}