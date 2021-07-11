#include "Model.h"
#include <cmath>
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include "TextureArray.h"

using DirectX::XMVECTOR;

namespace
{
	struct VertexType final
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
	};
}

struct ModelType final
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
	float tx, ty, tz;
	float bx, by, bz;
};

bool Model::Initialize(ID3D11Device* device,
	const std::filesystem::path& modelPath, const std::vector<std::filesystem::path>& texturePaths)
{
	if (!LoadModel(modelPath)) return false;

	CalcModelVectors();

	if (!InitBuffer(device)) return false;
	return LoadTextures(device, texturePaths);
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
	if (model)
	{
		delete[] model;
		model = nullptr;
	}

	if (textures)
	{
		textures->Release();
		delete textures;
		textures = nullptr;
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

bool Model::InitBuffer(ID3D11Device* device)
{
	std::vector<VertexType> vertices(vertexCount);
	std::vector<DWORD> indices(indexCount);

	for (UINT i = 0; i < vertexCount; ++i)
	{
		vertices[i].position = { model[i].x, model[i].y, model[i].z };
		vertices[i].texture = { model[i].tu, model[i].tv };
		vertices[i].normal = { model[i].nx, model[i].ny, model[i].nz };
		vertices[i].tangent = { model[i].tx, model[i].ty, model[i].tz };
		vertices[i].binormal = { model[i].bx, model[i].by, model[i].bz };

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

bool Model::LoadModel(const std::filesystem::path& path)
{
	constexpr static auto SkipToken = [](std::ifstream& fin, char token)
	{
		char input = 0;
		while (input != token)
			input = fin.get();
	};

	std::ifstream fin{ path };
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

bool Model::LoadTextures(ID3D11Device* device, const std::vector<std::filesystem::path>& paths)
{
	textures = new TextureArray{};
	if (!textures) return false;

	return textures->Initialize(device, paths);
}

void Model::CalcModelVectors()
{
	int faceCount, i, index;
	TempVertex vertex[3];
	Vector normal, tangent, binormal;
	
	int faceCount = vertexCount / 3;
	index = 0;
	
	for (int i = 0; i < faceCount; ++i)
	{
		memcpy(&vertex[0], &model[index++], sizeof(TempVertex));
		memcpy(&vertex[1], &model[index++], sizeof(TempVertex));
		memcpy(&vertex[2], &model[index++], sizeof(TempVertex));

		CalcTangentBinormal(vertex[0], vertex[1], vertex[2], tangent, binormal);
		normal = CalcNormal(tangent, binormal);

		memcpy(&model[index - 1].nx, &normal, sizeof(Vector) * 3);
		memcpy(&model[index - 2].nx, &normal, sizeof(Vector) * 3);
		memcpy(&model[index - 3].nx, &normal, sizeof(Vector) * 3);
	}
}

void Model::CalcTangentBinormal(const TempVertex& vertex1, const TempVertex& vertex2, const TempVertex& vertex3, Vector& tangent, Vector& binormal)
{
	float vector1[3], vector2[3];

	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	float tuVector[2], tvVector[2];

	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[1] = vertex2.tv - vertex1.tv;

	tuVector[0] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	const float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	tangent.x = (tuVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tuVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tuVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tvVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tvVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tvVector[1] * vector1[2]) * den;

	float length = std::sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	tangent.x /= length;
	tangent.y /= length;
	tangent.z /= length;

	length = std::sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	binormal.x /= length;
	binormal.y /= length;
	binormal.z /= length;
}

Model::Vector Model::CalcNormal(const Vector& tangent, const Vector& binormal)
{
	Vector normal;

	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	const float length = std::sqrt((normal.x * normal.x)
		* (normal.y * normal.y) * (normal.z * normal.z));

	normal.x /= length;
	normal.x /= length;
	normal.y /= length;

	return normal;
}
