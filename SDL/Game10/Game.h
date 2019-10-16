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
	inline class PhysWorld* GetPhysWorld() { return physWorld; }

	inline std::vector<class PlaneActor*>& GetPlanes() { return planes; }
	void AddPlane(PlaneActor* plane);
	void RemovePlane(PlaneActor* plane);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	Renderer* renderer;
	AudioSystem* audioSystem;
	InputSystem* inputSystem;
	PhysWorld* physWorld;

	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;

	std::vector<class PlaneActor*> planes;

	class FPSActor* fpsActor;
	class SpriteComponent* crosshair;

	SoundEvent musicEvent;
};