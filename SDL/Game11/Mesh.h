#pragma once

#include <vector>
#include <string>
#include "Collision.h"

class Mesh {
public:
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();

	inline class Texture* GetTexture(const size_t index) {
		return index < textures.size() ? textures[index] : nullptr;
	}

	inline class VertexArray* GetVertexArray() { return vertexArray; }

	inline const std::string& GetShaderName() const { return shaderName; }

	inline float GetRadius() const { return radius; }

	inline float GetSpecularPower() const { return specPower; }

	inline const AABB& GetBox() const { return box; }

private:
	std::vector<Texture*> textures;

	VertexArray* vertexArray;

	std::string shaderName;

	AABB box{ Vector3::Infinity, -Vector3::Infinity };

	float radius;

	float specPower;
};

