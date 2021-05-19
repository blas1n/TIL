#pragma once

#include "stdafx.h"

class Model final
{
public:
	Model()
		: vertexBuffer(nullptr)
		, indexBuffer(nullptr)
		, vertexCount(0)
		, indexCount(0) {}

	Model(const Model&) = default;
	Model(Model&&) noexcept = default;

	Model& operator=(const Model&) = default;
	Model& operator=(Model&&) noexcept = default;

	~Model() {}

	[[nodiscard]] bool Initialize(struct ID3D11Device* device);
	void Render(struct ID3D11DeviceContext* context);
	void Release() noexcept;

	UINT GetVertexCount() const noexcept { return vertexCount; }
	UINT GetIndexCount() const noexcept { return indexCount; }

private:
	struct ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	UINT vertexCount, indexCount;
};
