#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "AudioComponent.h"
#include "FPSActor.h"
#include "FollowActor.h"
#include "OrbitActor.h"
#include "PlaneActor.h"

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	renderer = new Renderer(this);
	if (!renderer->Initialize(1024.0f, 768.0f)) {
		SDL_Log("Failed to initialize renderer");
		return false;
	}

	audioSystem = new AudioSystem(this);
	if (!audioSystem->Initialize()) {
		SDL_Log("Failed to initialize audio system");
		return false;
	}

	inputSystem = new InputSystem();
	if (!inputSystem->Initialize()) {
		SDL_Log("Failed to initialize input system");
		return true;
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

	if (state.controllers.size() > 0) {
		if (state.controllers[0].GetButtonState(SDL_CONTROLLER_BUTTON_Y) == ButtonState::EPressed)
			ChangeCamera(1);
		else if (state.controllers[0].GetButtonState(SDL_CONTROLLER_BUTTON_X) == ButtonState::EPressed)
			ChangeCamera(2);
		else if (state.controllers[0].GetButtonState(SDL_CONTROLLER_BUTTON_A) == ButtonState::EPressed)
			ChangeCamera(3);
	}
	else {
		if (state.keyboard.GetKeyState(SDL_SCANCODE_1) == ButtonState::EPressed)
			ChangeCamera(1);
		else if (state.keyboard.GetKeyState(SDL_SCANCODE_2) == ButtonState::EPressed)
			ChangeCamera(2);
		else if (state.keyboard.GetKeyState(SDL_SCANCODE_3) == ButtonState::EPressed)
			ChangeCamera(3);
	}

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
	auto a = new Actor{ this };
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);

	Quaternion q{ Vector3::UnitY, -Math::Pi * 0.5f };
	q = Quaternion::Concatenate(q, Quaternion{ Vector3::UnitZ, Math::Pi * 0.25f + Math::Pi });
	a->SetRotation(q);

	auto mc = new MeshComponent{ a };
	mc->SetMesh(renderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor{ this };
	a->SetPosition(Vector3{ 200.0f, -75.0f, 0.0f });
	a->SetScale(3.0f);
	mc = new MeshComponent{ a };
	mc->SetMesh(renderer->GetMesh("Assets/Sphere.gpmesh"));

	constexpr auto start = -1250.0f;
	constexpr auto size = 250.0f;

	for (auto i = 0; i < 10; ++i) {
		for (auto j = 0; j < 10; ++j) {
			a = new PlaneActor(this);
			a->SetPosition(Vector3{ start + i * size, start + j * size, -100.0f });
		}
	}

	q = Quaternion{ Vector3::UnitX, Math::Pi * 0.5f };
	const auto p = Quaternion::Concatenate(q, Quaternion{ Vector3::UnitZ, Math::Pi * 0.5f });
	for (auto i = 0; i < 10; ++i) {
		a = new PlaneActor{ this };
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor{ this };
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor{ this };
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(p);

		a = new PlaneActor{ this };
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(p);
	}

	renderer->SetAmbientLight(Vector3::One * 0.2f);
	DirectionalLight& dir = renderer->GetDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.specularColor = Vector3(0.8f, 0.8f, 0.8f);

	fpsActor = new FPSActor{ this };
	followActor = new FollowActor{ this };
	orbitActor = new OrbitActor{ this };

	a = new Actor(this);
	a->SetScale(2.0f);
	crosshair = new SpriteComponent(a);
	crosshair->SetTexture(renderer->GetTexture("Assets/Crosshair.png"));

	a = new Actor{ this };
	a->SetPosition(Vector3{ -350.0f, -350.0f, 0.0f });
	auto sc = new SpriteComponent{ a };
	sc->SetTexture(renderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor{ this };
	a->SetPosition(Vector3{ 375.0f, -275.0f, 0.0f });
	a->SetScale(0.75f);
	sc = new SpriteComponent{ a };
	sc->SetTexture(renderer->GetTexture("Assets/Radar.png"));

	a = new Actor(this);
	a->SetPosition(Vector3{ 500.0f, -75.0f, 0.0f });
	a->SetScale(1.0f);
	mc = new MeshComponent{ a };
	mc->SetMesh(renderer->GetMesh("Assets/Sphere.gpmesh"));
	auto ac = new AudioComponent{ a };
	ac->PlayEvent("event:/FireLoop");

	musicEvent = audioSystem->PlayEvent("event:/Music");
	inputSystem->SetRelativeMouseMode(true);
	ChangeCamera(1);
}

void Game::UnloadData() {
	while (!actors.empty())
		delete actors.back();

	if (renderer)
		renderer->UnloadData();
}

void Game::ChangeCamera(const int mode) {
	fpsActor->SetState(Actor::State::EPaused);
	fpsActor->SetVisible(false);
	crosshair->SetVisible(false);
	followActor->SetState(Actor::State::EPaused);
	followActor->SetVisible(false);
	orbitActor->SetState(Actor::State::EPaused);
	orbitActor->SetVisible(false);

	switch (mode) {
	case 1:
	default:
		fpsActor->SetState(Actor::State::EActive);
		fpsActor->SetVisible(true);
		crosshair->SetVisible(true);
		break;
	case 2:
		followActor->SetState(Actor::State::EActive);
		followActor->SetVisible(true);
		break;
	case 3:
		orbitActor->SetState(Actor::State::EActive);
		orbitActor->SetVisible(true);
		break;
	}
}