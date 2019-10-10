#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(Game* game)
	: Actor(game), selectedTile(nullptr) {

	tiles.resize(NumRows);
	for (auto& tile : tiles)
		tile.resize(NumCols);

	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			tiles[i][j] = new Tile(game);
			const auto pos = Vector2{ j * TileSize + TileSize * 0.5f, StartY + i * TileSize };
			tiles[i][j]->SetPosition(pos);
		}
	}
	
	GetStartTile()->SetTileState(Tile::TileState::EStart);
	GetEndTile()->SetTileState(Tile::TileState::EBase);
	
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			if (i > 0)
				tiles[i][j]->adjacent.push_back(tiles[i-1][j]);
			if (i < NumRows - 1)
				tiles[i][j]->adjacent.push_back(tiles[i+1][j]);
			if (j > 0)
				tiles[i][j]->adjacent.push_back(tiles[i][j-1]);
			if (j < NumCols - 1)
				tiles[i][j]->adjacent.push_back(tiles[i][j+1]);
		}
	}
	
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());
	
	nextEnemy = EnemyTime;
}

void Grid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	
	if ((nextEnemy -= deltaTime) <= 0.0f) {
		new Enemy(GetGame());
		nextEnemy += EnemyTime;
	}
}

void Grid::ProcessClick(int x, int y) {
	y -= static_cast<int>(StartY - TileSize * 0.5f);

	if (y >= 0) {
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);

		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
			SelectTile(y, x);
	}
}

bool Grid::FindPath(Tile* start, Tile* goal) {
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			tiles[i][j]->g = 0.0f;
			tiles[i][j]->inOpenSet = false;
			tiles[i][j]->inClosedSet = false;
		}
	}
	
	std::vector<Tile*> openSet;
	
	Tile* current = start;
	current->inClosedSet = true;
	
	do {
		for (Tile* neighbor : current->adjacent) {
			if (neighbor->blocked | neighbor->inClosedSet)
				continue;

			if (!neighbor->inOpenSet) {
				neighbor->parent = current;
				neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
				neighbor->g = current->g + TileSize;
				neighbor->f = neighbor->g + neighbor->h;
				openSet.emplace_back(neighbor);
				neighbor->inOpenSet = true;
			}
			else {
				
				float newG = current->g + TileSize;
				if (newG < neighbor->g) {
					neighbor->parent = current;
					neighbor->g = newG;
					neighbor->f = neighbor->g + neighbor->h;
				}
			}
		}
		
		if (openSet.empty()) break;
		

		auto iter = std::min_element(
			openSet.begin(),
			openSet.end(),
			[](auto a, auto b) {
				return a->f < b->f;
			});

		current = *iter;
		openSet.erase(iter);
		current->inOpenSet = false;
		current->inClosedSet = true;
	} while (current != goal);
	
	return (current == goal) ? true : false;
}

void Grid::BuildTower() {
	if (selectedTile && !selectedTile->blocked) {
		selectedTile->blocked = true;

		if (FindPath(GetEndTile(), GetStartTile())) {
			auto t = new Tower(GetGame());
			t->SetPosition(selectedTile->GetPosition());
		}
		else {
			selectedTile->blocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}

		UpdatePathTiles(GetStartTile());
	}
}

void Grid::SelectTile(const size_t row, const size_t col) {
	const auto tstate = tiles[row][col]->GetTileState();

	if (tstate != Tile::TileState::EStart && tstate != Tile::TileState::EBase) {
		if (selectedTile)
			selectedTile->ToggleSelect();

		selectedTile = tiles[row][col];
		selectedTile->ToggleSelect();
	}
}

void Grid::UpdatePathTiles(Tile* start) {
	for (size_t i = 0; i < NumRows; i++)
		for (size_t j = 0; j < NumCols; j++)
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
				tiles[i][j]->SetTileState(Tile::TileState::EDefault);

	for (auto t = start->parent; t != GetEndTile(); t = t->parent)
		t->SetTileState(Tile::TileState::EPath);
}