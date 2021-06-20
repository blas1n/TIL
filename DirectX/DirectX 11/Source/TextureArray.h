#pragma once

#include <filesystem>

class TextureArray final
{
	constexpr static unsigned int TextureNum = 2;

public:
	bool Initialize(struct ID3D11Device* device, const std::filesystem::path paths[TextureNum]);
	void Release();

	struct ID3D11ShaderResourceView** GetTextures() noexcept { return textures; }

private:
	ID3D11ShaderResourceView* textures[TextureNum];
};
