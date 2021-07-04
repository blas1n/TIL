#pragma once

#include <filesystem>
#include <vector>

class TextureArray final
{
public:
	bool Initialize(struct ID3D11Device* device, const std::vector<std::filesystem::path>& paths);
	void Release();

	struct ID3D11ShaderResourceView** GetTextures() noexcept { return textures.data(); }
	size_t GetTextureNum() const noexcept { return textures.size(); }

private:
	std::vector<ID3D11ShaderResourceView*> textures;
};
