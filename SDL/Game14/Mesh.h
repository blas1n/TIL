#pragma once

#include <vector>
#include <string>
#include "Collision.h"

class Mesh {
public:
	bool Load(const std::string& inFileName, class Renderer* renderer);
	void Unload();

	inline class Texture* GetTexture(const size_t index) {
		return index < textures.size() ? textures[index] : nullptr;
	}

	inline class VertexArray* GetVertexArray() { return vertexArray; }

	inline const std::string& GetShaderName() const { return shaderName; }

	inline const std::string& GetFileName() const { return fileName; }

	inline float GetRadius() const { return radius; }

	inline float GetSpecularPower() const { return specPower; }

	inline const AABB& GetBox() const { return box; }

private:
	void SaveBinary(const std::string& fileName, const void* verts,
		uint32_t numVerts, VertexArray::Layout layout,
		const uint32_t* indices, uint32_t numIndices,
		const std::vector<std::string>& textureNames,
		const AABB& box, float radius,
		float specPower);

	bool LoadBinary(const std::string& fileName, Renderer* renderer);

	std::vector<Texture*> textures;

	VertexArray* vertexArray;

	std::string shaderName;

	std::string fileName;

	AABB box{ Vector3::Infinity, -Vector3::Infinity };

	float radius;

	float specPower;
};

