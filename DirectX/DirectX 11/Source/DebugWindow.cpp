#include "DebugWindow.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>

namespace
{
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
}

bool DebugWindow::Initialize(ID3D11Device* device, int inScreenWidth, int inScreenHeight, int inBitmapWidth, int inBitmapHeight)
{
	screenWidth = inScreenWidth;
	screenHeight = inScreenHeight;

	bitmapWidth = inBitmapWidth;
	bitmapHeight = inBitmapHeight;

	prevPosX = prevPosY = -1;

	vertexCount = 4;
	indexCount = 6;

	std::vector<VertexType> vertices(vertexCount);
	std::vector<UINT> indices{ 0, 1, 2, 0, 4, 1 };

	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(data));

	data.pSysMem = vertices.data();

	HRESULT result = device->CreateBuffer(&desc, &data, &vertexBuffer);
	if (FAILED(result)) return false;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	data.pSysMem = indices.data();

	result = device->CreateBuffer(&desc, &data, &indexBuffer);
	return SUCCEEDED(result);
}

bool DebugWindow::Render(ID3D11DeviceContext* context, int posX, int posY)
{
	bool result = UpdateBuffer(context, posX, posY);
	if (!result) return false;

	RenderBuffer(context);
	return true;
}

void DebugWindow::Release() noexcept
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

bool DebugWindow::UpdateBuffer(ID3D11DeviceContext* context, int posX, int posY)
{
	if ((posX == prevPosX) && (posY == prevPosY))
		return true;

	prevPosX = posX;
	prevPosY = posY;

	const float left = (static_cast<float>(-screenWidth) * 0.5f) + static_cast<float>(posX);
	const float top = (static_cast<float>(screenHeight) * 0.5f) - static_cast<float>(posY);

	const float right = left + static_cast<float>(bitmapWidth);
	const float bottom = top - static_cast<float>(bitmapHeight);
	
	std::vector<VertexType> vertices(vertexCount);
	vertices[0].position = { left, top, 0.0f };
	vertices[0].texture = { 0.0f, 0.0f };

	vertices[1].position = { right, bottom, 0.0f };
	vertices[1].texture = { 1.0f, 1.0f };
	
	vertices[2].position = { left, bottom, 0.0f };
	vertices[2].texture = { 0.0f, 1.0f };
	
	vertices[3].position = { right, top, 0.0f };
	vertices[3].texture = { 1.0f, 0.0f };

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	VertexType* verticesPtr = reinterpret_cast<VertexType*>(mappedResource.pData);
	memcpy(verticesPtr, vertices.data(), sizeof(VertexType) * vertexCount);

	context->Unmap(vertexBuffer, 0);
	return true;
}

void DebugWindow::RenderBuffer(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(VertexType), offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
