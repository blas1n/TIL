#pragma once

#include "Component.h"
#include "Mesh.h"

class MeshComponent : public Component {
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(Mesh* inMesh) { mesh = inMesh; }

	inline void SetTexutreIndex(size_t inTextureIndex) { textureIndex = inTextureIndex; }
	
	inline bool GetVisible() const { return visible; }
	inline void SetVisible(const bool inVisible) { visible = inVisible; }

protected:
	Mesh* mesh;
	size_t textureIndex;
	bool visible;
};