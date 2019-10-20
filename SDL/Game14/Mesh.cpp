#include "Mesh.h"
#include <limits>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "LevelLoader.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Math.h"

namespace {
	union Vertex {
		float f;
		uint8_t b[4];
	};

	constexpr int BinaryVersion = 1;
	struct MeshBinHeader {
		char mSignature[4] = { 'G', 'M', 'S', 'H' };
		uint32_t mVersion = BinaryVersion;
		VertexArray::Layout layout = VertexArray::Layout::PosNormTex;

		uint32_t numTextures = 0;
		uint32_t numVerts = 0;
		uint32_t numIndices = 0;

		AABB box{ Vector3::Zero, Vector3::Zero };
		float radius = 0.0f;
		float specPower = 100.0f;
	};
}

bool Mesh::Load(const std::string& inFileName, Renderer* renderer) {
	fileName = inFileName;

	if (LoadBinary(fileName + ".bin", renderer))
		return true;

	rapidjson::Document doc;
	if (!LevelLoader::LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load mesh %s", fileName.c_str());
		return false;
	}

	shaderName = doc["shader"].GetString();

	const auto& jsonTextures = doc["textures"];
	if (jsonTextures.IsArray()) {
		for (rapidjson::SizeType i = 0; i < jsonTextures.Size(); ++i) {
			const std::string name = jsonTextures[i].GetString();
			auto texture = renderer->GetTexture(name);

			if (texture == nullptr)
				texture = renderer->GetTexture("Assets/Default.png");

			textures.emplace_back(texture);
		}
	}

	const auto& jsonVerts = doc["vertices"];
	if (!jsonVerts.IsArray() || jsonVerts.Size() < 1) {
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	auto layout = VertexArray::Layout::PosNormTex;
	size_t vertSize = 8;

	const auto vertexFormat = doc["vertexformat"].GetString();
	if (!strcmp(vertexFormat, "PosNormSkinTex")) {
		layout = VertexArray::Layout::PosNormSkinTex;
		vertSize = 10;
	}

	std::vector<Vertex> vertices;
	vertices.reserve(jsonVerts.Size() * vertSize);
	
	for (rapidjson::SizeType i = 0; i < jsonVerts.Size(); ++i) {
		const auto& vert = jsonVerts[i];
		if (!vert.IsArray()) {
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		const Vector3 pos{ vert[0].GetFloat(), vert[1].GetFloat(), vert[2].GetFloat() };
		radius = Math::Max(radius, pos.LengthSquared());
		box.UpdateMinMax(pos);

		if (layout == VertexArray::Layout::PosNormTex) {
			Vertex v;
			for (rapidjson::SizeType j = 0; j < vert.Size(); ++j) {
				v.f = vert[j].GetFloat();
				vertices.emplace_back(v);
			}
		}
		else {
			Vertex v;
			rapidjson::SizeType j;
			for (j = 0; j < 6; ++j) {
				v.f = vert[j].GetFloat();
				vertices.emplace_back(v);
			}
			for (j = 6; j < 14; j += 4) {
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}
			for (j = 14; j < vert.Size(); ++j) {
				v.f = vert[j].GetFloat();
				vertices.emplace_back(v);
			}
		}
	}

	radius = Math::Sqrt(radius);

	const rapidjson::Value& jsonIndices = doc["indices"];
	if (!jsonIndices.IsArray() || jsonIndices.Size() < 1) {
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(jsonIndices.Size() * 3);
	for (rapidjson::SizeType i = 0; i < jsonIndices.Size(); ++i) {
		const auto& index = jsonIndices[i];
		if (!index.IsArray() || index.Size() != 3) {
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(index[0].GetUint());
		indices.emplace_back(index[1].GetUint());
		indices.emplace_back(index[2].GetUint());
	}

	specPower = doc["specularPower"].GetFloat();

	vertexArray = new VertexArray {
		vertices.data(),
		static_cast<unsigned int>(vertices.size()) / vertSize,
		layout,
		indices.data(),
		static_cast<unsigned int>(indices.size())
	};

	return true;
}

void Mesh::Unload() {
	delete vertexArray;
	vertexArray = nullptr;
}

void SaveBinary(const std::string& fileName, const void* verts,
	const uint32_t numVerts, const VertexArray::Layout layout,
	const uint32_t* indices, const uint32_t numIndices,
	const std::vector<std::string>& textureNames,
	const AABB& box, const float radius, const float specPower) {

	MeshBinHeader header;
	header.layout = layout;
	header.numTextures =
		static_cast<unsigned>(textureNames.size());
	header.numVerts = numVerts;
	header.numIndices = numIndices;
	header.box = box;
	header.radius = radius;
	header.specPower = specPower;

	std::ofstream outFile{ fileName, std::ios::out | std::ios::binary };
	if (!outFile.is_open()) return;
	
	outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

	for (const auto& tex : textureNames) {
		const auto nameSize = static_cast<uint16_t>(tex.length()) + 1;
		outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
		outFile.write(tex.c_str(), nameSize - 1);
		outFile.write("\0", 1);
	}

	const auto vertexSize = VertexArray::GetVertexSize(layout);
	outFile.write(reinterpret_cast<const char*>(verts), numVerts * vertexSize);
	outFile.write(reinterpret_cast<const char*>(indices), numIndices * sizeof(uint32_t));
}

bool Mesh::LoadBinary(const std::string& fileName, Renderer* renderer) {
	std::ifstream inFile{ fileName, std::ios::in | std::ios::binary };
	if (!inFile.is_open()) return false;
	
	MeshBinHeader header;
	inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

	const auto sig = header.mSignature;
	if (sig[0] != 'G' || sig[1] != 'M' || sig[2] != 'S' ||
			sig[3] != 'H' || header.mVersion != BinaryVersion)
		return false;
	
	char texName[std::numeric_limits<uint16_t>::max()];

	for (uint32_t i = 0; i < header.numTextures; i++) {
		uint16_t nameSize = 0;
		inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
		memset(texName, 0, sizeof(texName));
		inFile.read(texName, nameSize);

		auto t = renderer->GetTexture(texName);
		if (t == nullptr)
			t = renderer->GetTexture("Assets/Default.png");

		textures.emplace_back(t);
	}

	const auto vertexSize = VertexArray::GetVertexSize(header.layout);
	auto verts = new char[header.numVerts * vertexSize];
	inFile.read(verts, header.numVerts * vertexSize);

	auto indices = new uint32_t[header.numIndices];
	inFile.read(reinterpret_cast<char*>(indices),
		header.numIndices * sizeof(uint32_t));

	vertexArray = new VertexArray(verts, header.numVerts,
		header.layout, indices, header.numIndices);

	delete[] verts;
	delete[] indices;

	box = header.box;
	radius = header.radius;
	specPower = header.specPower;

	return true;
}