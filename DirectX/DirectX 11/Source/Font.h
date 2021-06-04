#pragma once

#include <filesystem>

class Font final
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device,
		const std::filesystem::path& fontPath, const std::filesystem::path& texturePath);

	void Release();

	void BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY);

	class Texture* GetTexture() const noexcept { return texture; }

private:
	bool LoadFontData(const std::filesystem::path& fontPath);
	bool LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath);

private:
	struct FontType* font;
	Texture* texture;
};
