#pragma once

#include "Actor.h"
#include <vector>

class Grid : public Actor {
public:
	Grid(class Game* game);

	void UpdateActor(float deltaTime) override;

	void ProcessClick(int x, int y);
	bool FindPath(class Tile* start, class Tile* goal);
	void BuildTower();
	
	inline class Tile* GetStartTile() { return tiles[3][0]; }
	inline class Tile* GetEndTile() { return tiles[3][15]; }

private:
	void SelectTile(size_t row, size_t col);
	void UpdatePathTiles(class Tile* start);
	
	std::vector<std::vector<class Tile*>> tiles;
	class Tile* selectedTile;

	float nextEnemy;
	
	constexpr static size_t NumRows = 7;
	constexpr static size_t NumCols = 16;

	constexpr static float StartY = 192.0f;

	constexpr static float TileSize = 64.0f;

	constexpr static float EnemyTime = 1.5f;
};
