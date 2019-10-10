#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include <iostream>

SpriteComponent::SpriteComponent(Actor* inOwner, const int inDrawOrder)
	: Component(inOwner),
	texture(nullptr),
	drawOrder(inDrawOrder),
	width(0),
	height(0) {

	owner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	owner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (texture) {
		SDL_Rect r;
		r.w = static_cast<int>(width * owner->GetScale());
		r.h = static_cast<int>(height * owner->GetScale());
		r.x = static_cast<int>(owner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(owner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(renderer,
			texture,
			nullptr,
			&r,
			-Math::ToDegrees(owner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* inTexture) {
	texture = inTexture;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}
