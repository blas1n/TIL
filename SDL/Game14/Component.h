#pragma once

#include <map>
#include <cstdint>
#include <rapidjson/document.h>

class Component {
public:
	enum class TypeId
	{
		TComponent,
		TAudioComponent,
		TBoxComponent,
		TCameraComponent,
		TFollowCamera,
		TMeshComponent,
		TMoveComponent,
		TSkeletalMeshComponent,
		TSpriteComponent,
		TMirrorCamera,
		TPointLightComponent,
		TTargetComponent,
		NUM_COMPONENT_TYPES
	};

	static std::map<TypeId, const char*> TypeNames;

	Component(class Actor* inOwner, int inUpdateOrder = 100);
	virtual ~Component();

	virtual void ProcessInput(const struct InputState& inputState) {}
	virtual void Update(float deltaTime) {}
	virtual void OnUpdateWorldTransform() {}

	inline Actor* GetOwner() { return owner; }
	inline int GetUpdateOrder() const { return updateOrder; }

	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const;

	template <typename T>
	static Component* Create(Actor* actor, const rapidjson::Value& inObj) {
		T* t = new T{ actor };
		t->LoadProperties(inObj);
		return t;
	}

	virtual TypeId GetType() const { return TypeId::TComponent; }

protected:
	class Actor* owner;
	int updateOrder;
};