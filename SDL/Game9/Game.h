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
	void RemoveActor(class Actor* actor);

	inline class Renderer* GetRenderer() { return renderer; }
	inline class AudioSystem* GetAudioSystem() { return audioSystem; }
	inline class InputSystem* GetInputSystem() { return inputSystem; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	void ChangeCamera(int mode);

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	Renderer* renderer;
	AudioSystem* audioSystem;
	InputSystem* inputSystem;

	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;

	class FPSActor* fpsActor;
	class FollowActor* followActor;
	class OrbitActor* orbitActor;
	class SpriteComponent* crosshair;

	SoundEvent musicEvent;
	SoundEvent reverbSnap;
};