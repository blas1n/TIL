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

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Texture* GetTexture(const std::string& fileName);

	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);

	std::vector<class Asteroid*>& GetAsteroids() { return asteroids; }

	void DeadShip();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	bool LoadShaders();
	void CreateSpriteVerts();

	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Texture*> textures;

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	std::vector<class SpriteComponent*> sprites;

	class Shader* spriteShader;
	class VertexArray* spriteVerts;

	SDL_Window* window;
	SDL_GLContext context;
	Uint32 ticksCount;

	bool isRunning;
	bool updatingActors;

	class Ship* ship;
	std::vector<class Asteroid*> asteroids;

	Vector3 curBgColor;
	Vector3 goalBgColor;

	float deadTimer;
};