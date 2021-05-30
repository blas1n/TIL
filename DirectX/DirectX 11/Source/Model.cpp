#include "Model.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include "Texture.h"

using DirectX::XMVECTOR;

struct VertexType final
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
};

struct ModelType final
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

bool Model::Initialize(ID3D11Device* device,
	const std::filesystem::path& modelPath, const std::filesystem::path& texturePath)
{
	if (!LoadModel(modelPath)) return false;
	if (!InitBuffer(device)) return false;
	return LoadTexture(device, texturePath);
}

void Model::ReadyToRender(struct ID3D11DeviceContext* context)
{
	constexpr UINT stride = sizeof(VertexType);
	constexpr UINT offset = 0;

	rot.y += DirectX::XM_PI * 0.005f;
	if (rot.y >= DirectX::XM_2PI)
		rot.y -= DirectX::XM_2PI;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Release() noexcept
{
	if (model)
	{
		delete[] model;
		model = nullptr;
	}

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

DirectX::XMMATRIX Model::GetWorldMatrix() const
{
	const auto rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rot));
	const auto scaleMat = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));

	const auto scaleRot = DirectX::XMMatrixMultiply(scaleMat, rotMat);
	const auto posMat = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos));

	return DirectX::XMMatrixMultiply(scaleRot, posMat);
}

bool Model::InitBuffer(ID3D11Device* device)
{
	std::vector<VertexType> vertices(vertexCount);
	std::vector<DWORD> indices(indexCount);

	for (UINT i = 0; i < vertexCount; ++i)
	{
		vertices[i].position = { model[i].x, model[i].y, model[i].z };
		vertices[i].texture = { model[i].tu, model[i].tv };
		vertices[i].normal = { model[i].nx, model[i].ny, model[i].nz };

		indices[i] = i;
	}

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

bool Model::LoadModel(const std::filesystem::path& modelPath)
{
	constexpr static auto SkipToken = [](std::ifstream& fin, char token)
	{
		char input = 0;
		while (input != token)
			input = fin.get();
	};

	std::ifstream fin{ modelPath };
	if (fin.fail()) return false;

	SkipToken(fin, ':');

	fin >> vertexCount;
	indexCount = vertexCount;

	model = new ModelType[vertexCount]{};
	if (!model) return false;

	SkipToken(fin, ':');
	(void)fin.get();
	(void)fin.get();

	for (UINT i = 0; i < vertexCount; ++i)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	fin.close();
	return true;
}

bool Model::LoadTexture(ID3D11Device* device, const std::filesystem::path& texturePath)
{
	texture = new Texture{};
	if (!texture) return false;

	return texture->Initialize(device, texturePath);
}
