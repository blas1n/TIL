#include "ModelClass.h"
#include <memory>

bool ModelClass::Init(ID3D11Device* device) {
	return InitBuffers(device);
}

void ModelClass::Release() {
	ReleaseBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount() {
	return m_indexCount;
}

bool ModelClass::InitBuffers(ID3D11Device* device) {
	m_vertexCount = 4;
	m_indexCount = 6;

	auto vertices = std::make_unique<VertexType[]>(m_vertexCount);
	if (!vertices) return false;

	auto indices = std::make_unique<ULONG[]>(m_indexCount);
	if (!indices) return false;

	vertices[0].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[3].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	D3D11_BUFFER_DESC vertexBufferDesc;
	
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_vertexCount * sizeof(VertexType);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) return false;

	D3D11_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = m_indexCount * sizeof(ULONG);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	
	return SUCCEEDED(result);
}

void ModelClass::ReleaseBuffers() {
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}