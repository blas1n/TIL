#pragma once

#include <string>
#include <unordered_map>
#include <SDL/SDL_ttf.h>
#include "Math.h"

class Font {
public:
	Font(class Game* inGame);

	bool Load(const std::string& fileName);
	void Unload();

	class Texture* RenderText(const std::string& textKey,
		const Vector3& color = Color::White,
		int pointSize = 30);

private:
	Game* game;

	std::unordered_map<int, TTF_Font*> fontData;
};

