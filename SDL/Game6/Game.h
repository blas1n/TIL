#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	inline class Renderer* GetRenderer() { return renderer; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	class Renderer* renderer;

	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;

	class CameraActor* cameraActor;
};