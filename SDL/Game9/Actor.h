#pragma once

#include <vector>
#include "Math.h"

class Component;

class Actor {
public:
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

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, rotation); }

	State GetState() const { return state; }
	void SetState(State inState) { state = inState; }

	class Game* GetGame() { return game; }

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

protected:
	virtual void ActorInput(const InputState& inputState) {}
	virtual void UpdateActor(float deltaTime) {}

private:
	State state;

	std::vector<Component*> components;
	class Game* game;

	Vector3 position;
	Quaternion rotation;
	float scale;

	Matrix4 worldTransform;
	bool recomputeWorldTransform;
};