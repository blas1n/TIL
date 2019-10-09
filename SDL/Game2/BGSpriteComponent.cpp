#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, const int drawOrder)
	:SpriteComponent(owner, drawOrder), scrollSpeed(0.0f) {}

void BGSpriteComponent::Update(const float deltaTime) {
	SpriteComponent::Update(deltaTime);

	for (auto& bg : BGTextures) {
		bg.offset.x += scrollSpeed * deltaTime;
		if (bg.offset.x < -screenSize.x)
			bg.offset.x = (BGTextures.size() - 1) * screenSize.x - 1;
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	for (auto& bg : BGTextures) {
		SDL_Rect r{
			static_cast<int>(owner->GetPosition().x - screenSize.x * 0.5f + bg.offset.x),
			static_cast<int>(owner->GetPosition().y - screenSize.y * 0.5f + bg.offset.y),
			static_cast<int>(screenSize.x),
			static_cast<int>(screenSize.y)
		};

		SDL_RenderCopy(renderer, bg.texture, nullptr, &r);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures) {
	int count = 0;

	for (auto texure : textures) {
		BGTexture temp;
		temp.texture = texure;
		temp.offset.x = count * screenSize.x;
		temp.offset.y = 0;
		BGTextures.emplace_back(temp);
		count++;
	}
}
