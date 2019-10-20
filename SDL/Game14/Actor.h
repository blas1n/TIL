#pragma once

#include <map>
#include <vector>
#include <rapidjson/document.h>
#include "Component.h"
#include "Math.h"

class Actor {
public:
	enum class TypeId {
		TActor,
		TFollowActor,
		TPlaneActor,
		TTargetActor,
		NUM_ACTOR_TYPES
	};
	
	static std::map<TypeId, const char*> TypeNames;
	
	enum class State {
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* inGame);
	virtual ~Actor();

	void ProcessInput(const struct InputState& inputState);
	void Update(float deltaTime);
	
#pragma region GetterAndSetter
	const Vector3& GetPosition() const { return position; }

	void SetPosition(const Vector3& inPosition) {
		position = inPosition;
		recomputeWorldTransform = true;
	}

	const Quaternion& GetRotation() const { return rotation; }

	void SetRotation(const Quaternion& inRotation) {
		rotation = inRotation;
		recomputeWorldTransform = true;
	}

	float GetScale() const { return scale; }

	void SetScale(float inScale) {
		scale = inScale;
		recomputeWorldTransform = true;
	}
#pragma endregion
		
	void ComputeWorldTransform();
	inline const Matrix4& GetWorldTransform() const { return worldTransform; }

	void RotateToNewForward(const Vector3& forward);

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, rotation); }
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, rotation); }
	Vector3 GetUp() const { return Vector3::Transform(Vector3::UnitZ, rotation); }

	State GetState() const { return state; }
	void SetState(State inState) { state = inState; }

	class Game* GetGame() { return game; }

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const;

	template <typename T>
	static Actor* Create(class Game* game, const rapidjson::Value& inObj) {
		T* t = new T(game);
		t->LoadProperties(inObj);
		return t;
	}

	Component* GetComponentOfType(Component::TypeId type) {
		Component* comp = nullptr;
		for (Component* c : components) {
			if (c->GetType() == type) {
				comp = c;
				break;
			}
		}

		return comp;
	}

	virtual TypeId GetType() const { return TypeId::TActor; }

	const std::vector<Component*>& GetComponents() const { return components; }

protected:
	virtual void ActorInput(const InputState& inputState) {}
	virtual void UpdateActor(float deltaTime) {}

private:
	State state;

	std::vector<Component*> components;
	Game* game;

	Vector3 position;
	Quaternion rotation;
	float scale;

	Matrix4 worldTransform;
	bool recomputeWorldTransform;
};