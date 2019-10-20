#pragma once

#include <vector>

class GBuffer {
public:
	enum class Type {
		EDiffuse,
		ENormal,
		EWorldPos,
		ESpecularPower,
		NUM_GBUFFER_TEXTURES
	};

	bool Create(int width, int height);
	void Destroy();

	class Texture* GetTexture(Type type);
	void SetTextureActive();

	inline unsigned int GetBufferId() const { return bufferId; }

private:
	std::vector<class Texture*> textures;
	unsigned int bufferId;
};