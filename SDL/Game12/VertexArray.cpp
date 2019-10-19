#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const void* verts, uint inNumVerts, Layout layout, const uint* indices, uint inNumIndices)
	: vertexBuffer(0), numVerts(inNumVerts), indexBuffer(0), numIndices(inNumIndices), vertexArray(0) {

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	uint vertexSize = 8 * sizeof(float);
	if (layout == Layout::PosNormSkinTex)
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint), indices, GL_STATIC_DRAW);

	switch (layout) {
	case Layout::PosNormTex: {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(6 * sizeof(float)));
		break;
	}
	case Layout::PosNormSkinTex: {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(6 * sizeof(float)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(6 * sizeof(float) + 4 * sizeof(char)));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(6 * sizeof(float) + 8 * sizeof(char)));
		break;
	}
	default: break;
	}
	
}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
}

void VertexArray::SetActive() {
	glBindVertexArray(vertexArray);
}