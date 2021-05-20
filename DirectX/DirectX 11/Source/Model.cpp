#include "Model.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using DirectX::XMVECTOR;

struct VertexType final
{
	XMVECTOR position;
	XMVECTOR color;
};

bool Model::Initialize(ID3D11Device* device)
{
	vertexCount = 3;
	indexCount = 3;

	DirectX::XMFLOAT3 position{ -1.0f, -1.0f, 0.0f };
	DirectX::XMFLOAT4 color{ 0.0f, 1.0f, 0.0f, 1.0f };
	vertices[0].position = DirectX::XMLoadFloat3(&position);
	vertices[0].color = DirectX::XMLoadFloat4(&color);

	position = { 0.0f, 1.0f, 0.0f };
	color = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertices[1].position = DirectX::XMLoadFloat3(&position);
	vertices[1].color = DirectX::XMLoadFloat4(&color);

	position = { 1.0f, -1.0f, 0.0f };
	color = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertices[2].position = DirectX::XMLoadFloat3(&position);
	vertices[2].color = DirectX::XMLoadFloat4(&color);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = vertexDesc.MiscFlags = vertexDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) return false;

	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(DWORD) * vertexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = indexDesc.MiscFlags = indexDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexDesc, &indexData, &indexBuffer);
	if (FAILED(result)) return false;

	return true;
}

void Model::ReadyToRender(struct ID3D11DeviceContext* context)
{
	constexpr UINT stride = sizeof(VertexType);
	constexpr UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Release() noexcept
{
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}
