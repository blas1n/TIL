#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Math.h"

namespace {
	union Vertex {
		float f;
		uint8_t b[4];
	};
}

bool Mesh::Load(const std::string& fileName, Renderer* renderer) {
	std::ifstream file{ fileName };
	if (!file.is_open()) {
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
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