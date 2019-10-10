#pragma once

#include "Component.h"
#include <SDL/SDL.h>

class SpriteComponent : public Component {
public:
	SpriteComponent(class Actor* inOwner, int inDrawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	inline int GetDrawOrder() const { return drawOrder; }
	inline int GetTextureWidth() const { return width; }
	inline int GetTextureHeight() const { return height; }

protected:
	SDL_Texture* texture;

	int drawOrder;
	int width;
	int height;
};
