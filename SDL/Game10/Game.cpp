#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "AudioComponent.h"
#include "FPSActor.h"
#include "TargetActor.h"
#include "PlaneActor.h"

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	renderer = new Renderer{ this };
	if (!renderer->Initialize(1024.0f, 768.0f)) {
		SDL_Log("Failed to initialize renderer");
		return false;
	}

	audioSystem = new AudioSystem{ this };
	if (!audioSystem->Initialize()) {
		SDL_Log("Failed to initialize audio system");
		return false;
	}

	inputSystem = new InputSystem();
	if (!inputSystem->Initialize()) {
		SDL_Log("Failed to initialize input system");
		return true;
	}

	physWorld = new PhysWorld{ this };

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
	
	if (renderer) {
		renderer->Shutdown();
		delete renderer;
		renderer = nullptr;
	}

	if (audioSystem) {
		audioSystem->Shutdown();
		delete audioSystem;
		audioSystem = nullptr;
	}

	if (inputSystem) {
		inputSystem->Shutdown();
		delete inputSystem;
		inputSystem = nullptr;
	}

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

void Game::AddPlane(PlaneActor* plane) {
	planes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane) {
	auto iter = std::find(planes.begin(), planes.end(), plane);
	if (iter != planes.end()) {
		std::iter_swap(iter, planes.end() - 1);
		planes.pop_back();
	}
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

	const auto deltaTime = Math::Min((SDL_GetTicks() - ticksCount) / 1000.0f, 0.05f);
	ticksCount = SDL_GetTicks();

	updatingActors = true;
	for (auto actor : actors)
		actor->Update(deltaTime);
	updatingActors = false;

	for (auto pending : pendingActors) {
		pending->ComputeWorldTransform();
		actors.emplace_back(pending);
	}

	pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : actors)
		if (actor->GetState() == Actor::State::EDead)
			deadActors.emplace_back(actor);

	for (auto actor : deadActors)
		delete actor;
	
	audioSystem->Update(deltaTime);
}

void Game::GenerateOutput() {
	renderer->Draw();
}

void Game::LoadData() {
	Actor* a = nullptr;
	Quaternion q;

	constexpr auto start = -1250.0f;
	constexpr auto size = 250.0f;

	for (auto i = 0; i < 10; ++i) {
		for (auto j = 0; j < 10; ++j) {
			a = new PlaneActor{ this };
			a->SetPosition(Vector3{ start + i * size, start + j * size, -100.0f });
		}
	}

	q = Quaternion{ Vector3::UnitX, Math::Pi * 0.5f };
	for (auto i = 0; i < 10; ++i)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3{ start + i * size, start - size, 0.0f });
		a->SetRotation(q);

		a = new PlaneActor{ this };
		a->SetPosition(Vector3{ start + i * size, -start + size, 0.0f });
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi * 0.5f));
	for (auto i = 0; i < 10; ++i) {
		a = new PlaneActor{ this };
		a->SetPosition(Vector3{ start - size, start + i * size, 0.0f });
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3{ -start + size, start + i * size, 0.0f });
		a->SetRotation(q);
	}

	renderer->SetAmbientLight(Vector3{ 0.2f, 0.2f, 0.2f });
	auto& dir = renderer->GetDirectionalLight();
	dir.direction = Vector3{ 0.0f, -0.707f, -0.707f };
	dir.diffuseColor = Vector3{ 0.78f, 0.88f, 1.0f };
	dir.specularColor = Vector3{ 0.8f, 0.8f, 0.8f };

	a = new Actor{ this };
	a->SetPosition(Vector3{ -350.0f, -350.0f, 0.0f });
	auto sc = new SpriteComponent{ a };
	sc->SetTexture(renderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor{ this };
	a->SetPosition(Vector3{ -390.0f, 275.0f, 0.0f });
	a->SetScale(0.75f);
	sc = new SpriteComponent{ a };
	sc->SetTexture(renderer->GetTexture("Assets/Radar.png"));

	a = new Actor{ this };
	a->SetScale(2.0f);
	crosshair = new SpriteComponent{ a };
	crosshair->SetTexture(renderer->GetTexture("Assets/Crosshair.png"));

	musicEvent = audioSystem->PlayEvent("event:/Music");

	fpsActor = new FPSActor(this);

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, 0.0f, 100.0f });
	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, 0.0f, 400.0f });
	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, -500.0f, 200.0f });
	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, 500.0f, 200.0f });

	inputSystem->SetRelativeMouseMode(true);
}

void Game::UnloadData() {
	while (!actors.empty())
		delete actors.back();

	if (renderer)
		renderer->UnloadData();
}