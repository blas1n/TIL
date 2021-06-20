#include "TextureArray.h"
#include <d3d11.h>
#include <DirectXTex.h>

bool TextureArray::Initialize(ID3D11Device* device, const std::filesystem::path paths[2])
{
	DirectX::ScratchImage image;
	HRESULT result;
	
	for (unsigned int i = 0; i < TextureNum; ++i)
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
	for (unsigned int i = 0; i < TextureNum; ++i)
	{
		if (textures[i])
		{
			textures[i]->Release();
			textures[i] = nullptr;
		}
	}
}
