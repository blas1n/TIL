#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent {
public:
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	void Update(float deltaTime) override;

	void Draw(SDL_Renderer* renderer) override;

	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	inline void SetScreenSize(const Vector2& size) { screenSize = size; }

	inline void SetScrollSpeed(const float speed) { scrollSpeed = speed; }

	inline float GetScrollSpeed() const { return scrollSpeed; }

private:
	struct BGTexture {
		SDL_Texture* texture;
		Vector2 offset;
	};

	std::vector<BGTexture> BGTextures;

	Vector2 screenSize;

	float scrollSpeed;
};
