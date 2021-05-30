#pragma once

#include "stdafx.h"
#include <filesystem>

class Model final
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device,
		const std::filesystem::path& moelPath, const std::filesystem::path& texturePath);

	void ReadyToRender(struct ID3D11DeviceContext* context);
	void Release() noexcept;

	class Texture* GetTexture() const noexcept { return texture; }
	UINT GetVertexCount() const noexcept { return vertexCount; }
	UINT GetIndexCount() const noexcept { return indexCount; }

private:
	bool InitBuffer(ID3D11Device* device);
	bool LoadModel(const std::filesystem::path& modelPath);
	bool LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath);

private:
	struct ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	struct ModelType* model;
	Texture* texture;
	
	UINT vertexCount, indexCount;
};
