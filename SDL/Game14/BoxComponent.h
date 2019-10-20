#pragma once

#include "Component.h"
#include "Collision.h"

class BoxComponent : public Component {
public:
	BoxComponent(Actor* inOwner, int inUpdateOrder = 100);
	~BoxComponent();

	void OnUpdateWorldTransform() override;

	inline void SetObjectBox(const AABB& model) { objectBox = model; };
	inline const AABB& GetWorldBox() const { return worldBox; }

	inline void SetShouldRotate(const bool value) { shouldRotate = value; };

	TypeId GetType() const override { return TypeId::TBoxComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	AABB objectBox;
	AABB worldBox;
	bool shouldRotate;
};