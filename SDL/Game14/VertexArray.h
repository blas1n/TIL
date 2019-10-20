#pragma once

class VertexArray {
public:
	using uint = unsigned int;

	enum class Layout {
		PosNormTex,
		PosNormSkinTex
	};

	VertexArray(const void* verts, uint inNumVerts, Layout layout, const uint* indices, uint inNumIndices);
	~VertexArray();

	void SetActive();

	inline uint GetNumVerts() const { return numIndices; }
	inline uint GetNumIndices() const { return numIndices; }

	static unsigned int GetVertexSize(VertexArray::Layout layout);

private:
	uint vertexBuffer;
	uint numVerts;

	uint indexBuffer;
	uint numIndices;

	uint vertexArray;
};

