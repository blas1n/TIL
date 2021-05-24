#pragma once

#include <filesystem>

class Texture final
{
public:
	bool Initialize(struct ID3D11Device* device, const std::filesystem::path& path);
	void Release();

	struct ID3D11ShaderResourceView* GetTexture() const noexcept { return texture; }

private:
	ID3D11ShaderResourceView* texture;
};
