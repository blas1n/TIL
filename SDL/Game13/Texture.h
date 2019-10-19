#pragma once

#include <string>

class Texture {
public:
	bool Load(const std::string& fileName);
	void Unload();
	void CreateFromSurface(struct SDL_Surface* surface);
	void CreateForRendering(int inWidth, int inHeight, unsigned int format);

	void SetActive(int index = 0);

	inline unsigned int GetTextureId() const { return textureId; }

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

private:
	unsigned int textureId;

	int width;
	int height;
};

