#pragma once

#include "stdafx.h"
#include <DirectXMath.h>
#include <filesystem>

class Bitmap final
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device, SIZE inScreenSize,
		const std::filesystem::path& texturePath, SIZE inBitmapSize);

	void ReadyToRender(struct ID3D11DeviceContext* context);
	void Release() noexcept;

	DirectX::XMMATRIX GetWorldMatrix() const;

	void SetPos(const DirectX::XMFLOAT3& inPos) noexcept { pos = inPos; }
	void SetRot(const DirectX::XMFLOAT3& inRot) noexcept { rot = inRot; }
	void SetScale(const DirectX::XMFLOAT3& inScale) noexcept { scale = inScale; }

	const DirectX::XMFLOAT3& GetPos() const noexcept { return pos; }
	const DirectX::XMFLOAT3& GetRot() const noexcept { return rot; }
	const DirectX::XMFLOAT3& GetScale() const noexcept { return scale; }

	class Texture* GetTexture() const noexcept { return texture; }
	UINT GetVertexCount() const noexcept { return vertexCount; }
	UINT GetIndexCount() const noexcept { return indexCount; }

private:
	bool InitBuffer(ID3D11Device* device);
	bool LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath);

private:
	DirectX::XMFLOAT3 pos, rot;
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	struct ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	struct ModelType* model = nullptr;
	Texture* texture = nullptr;

	DirectX::XMFLOAT2 screenSize, bitmapSize;
	UINT vertexCount = 0, indexCount = 0;
};
