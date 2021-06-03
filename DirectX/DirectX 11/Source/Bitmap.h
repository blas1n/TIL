#pragma once

#include "stdafx.h"
#include "Object.h"
#include <filesystem>

class Bitmap final : public Object
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device, SIZE inScreenSize,
		const std::filesystem::path& texturePath, SIZE inBitmapSize);

	void ReadyToRender(struct ID3D11DeviceContext* context);
	void Release() noexcept;

	DirectX::XMMATRIX GetWorldMatrix() const override;

	class Texture* GetTexture() const noexcept { return texture; }
	UINT GetVertexCount() const noexcept { return vertexCount; }
	UINT GetIndexCount() const noexcept { return indexCount; }

private:
	bool InitBuffer(ID3D11Device* device);
	bool LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath);

private:
	struct ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	struct ModelType* model = nullptr;
	Texture* texture = nullptr;

	DirectX::XMFLOAT2 screenSize, bitmapSize;
	UINT vertexCount = 0, indexCount = 0;
};
