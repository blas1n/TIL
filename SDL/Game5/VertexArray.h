#pragma once

class VertexArray {
public:
	using uint = unsigned int;

	VertexArray(const float* verts, uint inNumVerts, const uint* indices, uint inNumIndices);
	~VertexArray();

	void SetActive();

	inline uint GetNumVerts() const { return numIndices; }
	inline uint GetNumIndices() const { return numIndices; }

private:
	uint vertexBuffer;
	uint numVerts;

	uint indexBuffer;
	uint numIndices;

	uint vertexArray;
};

