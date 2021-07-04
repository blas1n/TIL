#include "TextureArray.h"
#include <d3d11.h>
#include <DirectXTex.h>

bool TextureArray::Initialize(ID3D11Device* device, const std::vector<std::filesystem::path>& paths)
{
	const size_t size = paths.size();
	textures.resize(size, nullptr);

	DirectX::ScratchImage image;
	HRESULT result;
	
	for (unsigned int i = 0; i < size; ++i)
	{
		result = DirectX::LoadFromDDSFile(
			paths[i].c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);

		if (FAILED(result)) return false;

		result = DirectX::CreateShaderResourceView(device,
			image.GetImages(), image.GetImageCount(), image.GetMetadata(), &textures[i]);

		if (FAILED(result)) return false;
	}

	return true;
}

void TextureArray::Release()
{
	const size_t size = textures.size();

	for (unsigned int i = 0; i < size; ++i)
		if (textures[i])
			textures[i]->Release();

	textures.clear();
}
