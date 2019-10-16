#include "Game.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <SDL/SDL_ttf.h>
#include <rapidjson/document.h>
#include "Font.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "PhysWorld.h"
#include "UiScreen.h"
#include "Actor.h"
#include "HUD.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "AudioComponent.h"
#include "FPSActor.h"
#include "TargetActor.h"
#include "PlaneActor.h"
#include "PauseMenu.h"

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

	if (TTF_Init() != 0) {
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	LoadData();
	ticksCount = SDL_GetTicks();
	gameState = GameState::EGameplay;
	return true;
}

void Game::RunLoop() {
	while (gameState != GameState::EQuit) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	UnloadData();
	TTF_Quit();
	delete physWorld;

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

void Game::PushUi(UiScreen* screen) {
	uiStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName) {
	auto iter = fonts.find(fileName);
	if (iter != fonts.end())
		return iter->second;

	auto font = new Font{ this };
	if (font->Load(fileName))
		fonts.emplace(fileName, font);
	else {
		font->Unload();
		delete font;
		font = nullptr;
	}

	return font;
}

void Game::LoadText(const std::string& fileName) {
	text.clear();
	
	std::ifstream file{ fileName };
	if (!file.is_open()) {
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents{ fileStream.str() };

	rapidjson::StringStream jsonStr{ contents.c_str() };
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}

	const auto& actions{ doc["TextMap"] };
	for (auto itr = actions.MemberBegin(); itr != actions.MemberEnd(); ++itr)
		if (itr->name.IsString() && itr->value.IsString())
			text.emplace(itr->name.GetString(), itr->value.GetString());
}

const std::string& Game::GetText(const std::string& key) {
	static std::string errorMsg("**KEY NOT FOUND**");

	auto iter = text.find(key);
	if (iter != text.cend())
		return iter->second;
	else
		return errorMsg;
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
			gameState = GameState::EQuit;
			break;
		default:
			inputSystem->ProcessEvent(event);
			break;
		}
	}

	inputSystem->Update();
	const auto state = inputSystem->GetState();

	if (gameState == GameState::EGameplay) {
		HandleInput(state);

		for (auto actor : actors)
			if (actor->GetState() == Actor::State::EActive)
				actor->ProcessInput(state);
	}
	else if (!uiStack.empty())
		uiStack.back()->ProcessInput(state);
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	const auto deltaTime = Math::Min((SDL_GetTicks() - ticksCount) / 1000.0f, 0.05f);
	ticksCount = SDL_GetTicks();

	if (gameState == GameState::EGameplay) {
		updatingActors = true;
		for (auto actor : actors)
			if (actor->GetState() == Actor::State::EActive)
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
	}
	
	audioSystem->Update(deltaTime);

	for (auto ui : uiStack)
		if (ui->GetState() == UiScreen::UiState::EActive)
			ui->Update(deltaTime);

	
	for (auto iter = uiStack.begin(); iter != uiStack.end();) {
		if ((*iter)->GetState() == UiScreen::UiState::EClosing) {
			delete *iter;
			iter = uiStack.erase(iter);
		}
		else ++iter;
	}
}

void Game::GenerateOutput() {
	renderer->Draw();
}

void Game::LoadData() {
	LoadText("Assets/English.gptext");

	Actor* a = nullptr;
	auto q = Quaternion::Identity;

	constexpr auto start = -1250.0f;
	constexpr auto size = 250.0f;

	for (auto i = 0; i < 10; ++i) {
		for (auto j = 0; j < 10; ++j) {
			a = new PlaneActor{ this };
			a->SetPosition(Vector3{ start + i * size, start + j * size, -100.0f });
		}
	}

	q = Quaternion{ Vector3::UnitX, Math::Pi * 0.5f };
	for (auto i = 0; i < 10; ++i) {
		a = new PlaneActor{ this };
		a->SetPosition(Vector3{ start + i * size, start - size, 0.0f });
		a->SetRotation(q);

		a = new PlaneActor{ this };
		a->SetPosition(Vector3{ start + i * size, -start + size, 0.0f });
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion{ Vector3::UnitZ, Math::Pi * 0.5f });

	for (auto i = 0; i < 10; ++i) {
		a = new PlaneActor{ this };
		a->SetPosition(Vector3{ start - size, start + i * size, 0.0f });
		a->SetRotation(q);

		a = new PlaneActor{ this };
		a->SetPosition(Vector3{ -start + size, start + i * size, 0.0f });
		a->SetRotation(q);
	}

	renderer->SetAmbientLight(Vector3{ 0.2f, 0.2f, 0.2f });
	auto& dir = renderer->GetDirectionalLight();
	dir.direction = Vector3{ 0.0f, -0.707f, -0.707f };
	dir.diffuseColor = Vector3{ 0.78f, 0.88f, 1.0f };
	dir.specularColor = Vector3{ 0.8f, 0.8f, 0.8f };
	
	hud = new HUD{ this };

	musicEvent = audioSystem->PlayEvent("event:/Music");

	fpsActor = new FPSActor{ this };

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, 0.0f, 100.0f });

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, 0.0f, 400.0f });

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, -500.0f, 200.0f });

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 1450.0f, 500.0f, 200.0f });

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 0.0f, -1450.0f, 200.0f });
	a->SetRotation(Quaternion{ Vector3::UnitZ, Math::Pi * 0.5f });

	a = new TargetActor{ this };
	a->SetPosition(Vector3{ 0.0f, 1450.0f, 200.0f });
	a->SetRotation(Quaternion{ Vector3::UnitZ, Math::Pi * -0.5f });

	inputSystem->SetRelativeMouseMode(true);
}

void Game::UnloadData() {
	while (!actors.empty())
		delete actors.back();

	while (!uiStack.empty()) {
		delete uiStack.back();
		uiStack.pop_back();
	}

	if (renderer)
		renderer->UnloadData();
}

void Game::HandleInput(const InputState& inputState) {
	const auto OnRelease = [inputState](auto code) {
		return inputState.keyboard.GetKeyState(code) == ButtonState::EReleased;
	};

	if (OnRelease(SDL_SCANCODE_ESCAPE))
		new PauseMenu(this);
	if (OnRelease(SDL_SCANCODE_MINUS)) {
		float volume = audioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		audioSystem->SetBusVolume("bus:/", volume);
	}
	if (OnRelease(SDL_SCANCODE_EQUALS)) {
		float volume = audioSystem->GetBusVolume("bus:/");
		volume = Math::Max(1.0f, volume + 0.1f);
		audioSystem->SetBusVolume("bus:/", volume);
	}
	if (OnRelease(SDL_SCANCODE_1))
		LoadText("Assets/English.gptext");
	if (OnRelease(SDL_SCANCODE_2))
		LoadText("Assets/Russian.gptext");
}