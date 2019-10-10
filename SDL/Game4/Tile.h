#pragma once

#include "Actor.h"
#include <vector>

class Tile : public Actor {
public:
	friend class Grid;
	enum class TileState {
		EDefault,
		EPath,
		EStart,
		EBase
	};
	
	Tile(class Game* game);

	TileState GetTileState() const { return tileState; }
	void SetTileState(TileState state);
	
	const Tile* GetParent() const { return parent; }

	void ToggleSelect();

private:
	void UpdateTexture();

private:
	std::vector<Tile*> adjacent;
	TileState tileState;

	class SpriteComponent* sprite;
	Tile* parent;

	float f;
	float g;
	float h;
	
	bool inOpenSet;
	bool inClosedSet;
	bool blocked;
	bool selected;
};
