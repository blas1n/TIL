#include "Font.h"
#include "Game.h"
#include "Texture.h"
#include <vector>

Font::Font(Game* inGame) 
	: game(inGame), fontData() {}

bool Font::Load(const std::string& fileName) {
	int fontSizes[] {
		8, 9, 10, 11, 12,
		14, 16, 18, 20, 22,
		24, 26, 28, 30, 32,
		34, 36, 38, 40, 42,
		44, 46, 48, 52, 56,
		60, 64, 68, 72
	};

	for (auto& size : fontSizes) {
		const auto font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr) {
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}

		fontData.emplace(size, font);
	}

	return true;
}

void Font::Unload() {
	for (auto& font : fontData)
		TTF_CloseFont(font.second);
}

Texture* Font::RenderText(const std::string& textKey,
	const Vector3& color /*= Color::White*/,
	int pointSize /*= 24*/) {

	Texture* texture = nullptr;

	const SDL_Color sdlColor{
		static_cast<Uint8>(color.x * 255),
		static_cast<Uint8>(color.y * 255),
		static_cast<Uint8>(color.z * 255),
		255
	};

	auto iter = fontData.find(pointSize);
	if (iter != fontData.end()) {
		const auto font = iter->second;
		const auto& actualText = game->GetText(textKey);
		const auto surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);

		if (surf != nullptr) {
			texture = new Texture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
		SDL_Log("Point size %d is unsupported", pointSize);

	return texture;
}