#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(Game* game)
	: Actor(game),
	adjacent(),
	tileState(TileState::EDefault),
	sprite(new SpriteComponent(this)),
	parent(nullptr),
	f(0.0f),
	g(0.0f),
	h(0.0f),
	inOpenSet(false),
	inClosedSet(false),
	blocked(false),
	selected(false) {

	UpdateTexture();
}

void Tile::SetTileState(TileState state) {
	tileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect() {
	selected = !selected;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	std::string text;
	switch (tileState) {
	case TileState::EStart:
			text = "Assets/TileTan.png";
			break;

	case TileState::EBase:
		text = "Assets/TileGreen.png";
		break;

	case TileState::EPath:
		if (selected)
			text = "Assets/TileGreySelected.png";
		else
			text = "Assets/TileGrey.png";
		break;

	case TileState::EDefault:
	default:
		if (selected)
			text = "Assets/TileBrownSelected.png";
		else
			text = "Assets/TileBrown.png";
		break;
	}

	sprite->SetTexture(GetGame()->GetTexture(text));
}
