#include "Texture.h"
#include <d3d11.h>
#include <DirectXTex.h>

bool Texture::Initialize(ID3D11Device* device, const std::filesystem::path& path)
{
	DirectX::ScratchImage image;
	HRESULT result = DirectX::LoadFromDDSFile(
		path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	
	if (FAILED(result)) return false;
	
	result = DirectX::CreateShaderResourceView(device,
		image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);

	return SUCCEEDED(result);
}

void Texture::Release()
{
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}
}
