#include "Bitmap.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "Texture.h"

namespace
{
	struct VertexType final
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
}

bool Bitmap::Initialize(ID3D11Device* device, SIZE inScreenSize,
	const std::filesystem::path& texturePath, SIZE inBitmapSize)
{
	screenSize = { static_cast<float>(inScreenSize.cx), static_cast<float>(inBitmapSize.cy) };
	bitmapSize = { static_cast<float>(inBitmapSize.cx), static_cast<float>(inBitmapSize.cy) };

	bool result = InitBuffer(device);
	if (!result) return false;

	result = LoadTexture(device, texturePath);
	if (!result) return false;

	return true;
}

void Bitmap::ReadyToRender(ID3D11DeviceContext* context)
{
	constexpr UINT stride = sizeof(VertexType);
	constexpr UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Bitmap::Release() noexcept
{
	if (texture)
	{
		texture->Release();
		delete texture;
		texture = nullptr;
	}

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

DirectX::XMMATRIX Bitmap::GetWorldMatrix() const
{
	const auto rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rot));

	DirectX::XMVECTOR realScale = DirectX::XMLoadFloat3(&scale);
	realScale = DirectX::XMVectorMultiply(realScale, DirectX::XMLoadFloat2(&bitmapSize));

	const auto scaleMat = DirectX::XMMatrixScalingFromVector(realScale);
	const auto scaleRot = DirectX::XMMatrixMultiply(scaleMat, rotMat);

	DirectX::XMVECTOR realPos = DirectX::XMLoadFloat3(&pos);
	realPos = DirectX::XMVectorMultiply(realPos, DirectX::XMLoadFloat2(&screenSize));

	const auto posMat = DirectX::XMMatrixTranslationFromVector(realPos);
	return DirectX::XMMatrixMultiply(scaleRot, posMat);
}

bool Bitmap::InitBuffer(ID3D11Device* device)
{
	vertexCount = 4;
	indexCount = 6;

	std::vector<VertexType> vertices(vertexCount);
	std::vector<DWORD> indices{ 0, 1, 2, 1, 3, 2 };

	vertices[0].position = { -1.0f, 1.0f, 0.0f };
	vertices[0].texture = { 0.0f, 0.0f };

	vertices[1].position = { 1.0f, 1.0f, 0.0f };
	vertices[1].texture = { 1.0f, 0.0f };

	vertices[2].position = { -1.0f, -1.0f, 0.0f };
	vertices[2].texture = { 0.0f, 1.0f };

	vertices[3].position = { 1.0f, -1.0f, 0.0f };
	vertices[3].texture = { 1.0f, 1.0f };

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
	indexDesc.ByteWidth = sizeof(DWORD) * indexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = indexDesc.MiscFlags = indexDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexDesc, &indexData, &indexBuffer);
	return SUCCEEDED(result);
}

bool Bitmap::LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath)
{
	texture = new Texture{};
	if (!texture) return false;

	return texture->Initialize(device, texturePath);
}
