#pragma once

#include "Component.h"
#include "Mesh.h"

class MeshComponent : public Component {
public:
	MeshComponent(Actor* inOwner, bool inIsSkeletal = false);
	~MeshComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(Mesh* inMesh) { mesh = inMesh; }

	inline void SetTexutreIndex(size_t inTextureIndex) { textureIndex = inTextureIndex; }
	
	inline bool GetVisible() const { return visible; }
	inline void SetVisible(const bool inVisible) { visible = inVisible; }

	inline bool IsSkeletal() const { return isSkeletal; }

	TypeId GetType() const override { return TypeId::TMeshComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

protected:
	Mesh* mesh;
	size_t textureIndex;
	bool visible;
	bool isSkeletal;
};