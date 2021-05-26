#pragma once

#include "stdafx.h"
#include <filesystem>

class Model final
{
public:
	Model()
		: vertexBuffer(nullptr)
		, indexBuffer(nullptr)
		, texture(nullptr)
		, vertexCount(0)
		, indexCount(0) {}

	Model(const Model&) = default;
	Model(Model&&) noexcept = default;

	Model& operator=(const Model&) = default;
	Model& operator=(Model&&) noexcept = default;

	~Model() {}

	[[nodiscard]] bool Initialize(struct ID3D11Device* device, const std::filesystem::path& texturePath);
	void ReadyToRender(struct ID3D11DeviceContext* context);
	void Release() noexcept;

	class Texture* GetTexture() const noexcept { return texture; }
	UINT GetVertexCount() const noexcept { return vertexCount; }
	UINT GetIndexCount() const noexcept { return indexCount; }

private:
	bool InitBuffer(ID3D11Device* device);
	bool LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath);

private:
	struct ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Texture* texture;
	
	UINT vertexCount, indexCount;
};
