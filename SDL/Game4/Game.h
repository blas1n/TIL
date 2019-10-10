#pragma once

#include <SDL/SDL.h>
#include "Math.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game {
public:
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	inline std::vector<class Enemy*>& GetEnemies() { return enemies; }
	inline class Grid* GetGrid() { return grid; }
	
	Enemy* GetNearestEnemy(const Vector2& pos);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> textures;

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	std::vector<class SpriteComponent*> sprites;

	SDL_Window* window;
	SDL_Renderer* renderer;
	Uint32 ticksCount;

	bool isRunning;
	bool updatingActors;

	std::vector<Enemy*> enemies;
	Grid* grid;
	float nextEnemy;
};