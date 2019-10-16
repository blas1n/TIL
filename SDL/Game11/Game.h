#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "SoundEvent.h"
#include "Math.h"

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(Actor* actor);

	inline class Renderer* GetRenderer() { return renderer; }
	inline class AudioSystem* GetAudioSystem() { return audioSystem; }
	inline class InputSystem* GetInputSystem() { return inputSystem; }
	inline class PhysWorld* GetPhysWorld() { return physWorld; }
	inline class HUD* GetHUD() { return hud; }

	void PushUi(class UiScreen* screen);
	inline const std::vector<UiScreen*>& GetUiStack() { return uiStack; }
	
	inline class FPSActor* GetPlayer() { return fpsActor; }

	enum class GameState {
		EGameplay,
		EPaused,
		EQuit
	};

	inline GameState GetState() const { return gameState; }
	inline void SetState(GameState state) { gameState = std::move(state); }

	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	inline std::vector<class PlaneActor*>& GetPlanes() { return planes; }
	void AddPlane(PlaneActor* plane);
	void RemovePlane(PlaneActor* plane);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	void HandleInput(const struct InputState& inputState);

	std::vector<Actor*> actors;
	std::vector<Actor*> pendingActors;

	std::vector<UiScreen*> uiStack;
	std::unordered_map<std::string, Font*> fonts;
	std::unordered_map<std::string, std::string> text;

	Renderer* renderer;
	AudioSystem* audioSystem;
	InputSystem* inputSystem;
	PhysWorld* physWorld;
	HUD* hud;

	Uint32 ticksCount;
	GameState gameState;
	bool updatingActors;

	std::vector<class PlaneActor*> planes;

	FPSActor* fpsActor;
	class SpriteComponent* crosshair;

	SoundEvent musicEvent;
};