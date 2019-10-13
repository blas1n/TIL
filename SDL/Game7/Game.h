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

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	void HandleKeyPress(int key);

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	Renderer* renderer;
	AudioSystem* audioSystem;

	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;

	class CameraActor* cameraActor;
	SoundEvent musicEvent;
	SoundEvent reverbSnap;
};