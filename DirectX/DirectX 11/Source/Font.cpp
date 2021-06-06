#include "Font.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <fstream>
#include "Texture.h"

namespace
{
	struct VertexType final
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
}

struct FontType
{
	float left, right;
	int size;
};

bool Font::Initialize(ID3D11Device* device,
	const std::filesystem::path& fontPath,
	const std::filesystem::path& texturePath)
{
	const bool result = LoadFontData(fontPath);
	if (!result) return false;
	
	return LoadTexture(device, texturePath);
}

void Font::Release()
{
	if (texture)
	{
		texture->Release();
		delete texture;
		texture = nullptr;
	}
	
	if (font)
	{
		delete[] font;
		font = nullptr;
	}
}

void Font::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	const auto vertexPtr = reinterpret_cast<VertexType*>(vertices);
	const int numLetters = static_cast<int>(strlen(sentence));
	int index = 0;

	for (int i = 0; i < numLetters; ++i)
	{
		const int letter = static_cast<int>(sentence[i]) - 32;
		if (letter == 0)
		{
			drawX += 3.0f;
			continue;
		}

		vertexPtr[index].position = { drawX, drawY, 0.0f };
		vertexPtr[index].texture = { font[letter].left, 0.0f };
		++index;

		vertexPtr[index].position = { drawX + font[letter].size, drawY - 16.0f, 0.0f };
		vertexPtr[index].texture = { font[letter].right, 1.0f };
		++index;

		vertexPtr[index].position = { drawX, drawY - 16.0f, 0.0f };
		vertexPtr[index].texture = { font[letter].left, 1.0f };
		++index;

		vertexPtr[index].position = { drawX, drawY, 0.0f };
		vertexPtr[index].texture = { font[letter].left, 0.0f };
		++index;

		vertexPtr[index].position = { drawX + font[letter].size, drawY, 0.0f };
		vertexPtr[index].texture = { font[letter].right, 0.0f };
		++index;

		vertexPtr[index].position = { drawX + font[letter].size, drawY - 16.0f, 0.0f };
		vertexPtr[index].texture = { font[letter].right, 1.0f };
		++index;

		drawX += font[letter].size + 1.0f;
	}
}

bool Font::LoadFontData(const std::filesystem::path& fontPath)
{
	constexpr static int FontNum = 95;

	font = new FontType[FontNum]{};
	if (!font) return false;

	std::ifstream fin{ fontPath };
	if (fin.fail()) return false;

	for (int i = 0; i < FontNum; ++i)
	{
		char input = fin.get();
		while (input != ' ')
			input = fin.get();

		input = fin.get();
		while (input != ' ')
			input = fin.get();

		fin >> font[i].left >> font[i].right >> font[i].size;
	}

	fin.close();
	return true;
}

bool Font::LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath)
{
	texture = new Texture{};
	if (!texture) return false;

	return texture->Initialize(device, texturePath);
}
