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

	Actor(class Game* game);
	virtual ~Actor();

	void ProcessInput(const struct InputState& inputState);
	void Update(float deltaTime);
	
#pragma region GetterAndSetter
	const Vector2& GetPosition() const { return position; }

	void SetPosition(const Vector2& inPosition) {
		position = inPosition;
		recomputeWorldTransform = true;
	}

	float GetScale() const { return scale; }

	void SetScale(float inScale) {
		scale = inScale;
		recomputeWorldTransform = true;
	}

	float GetRotation() const { return rotation; }

	void SetRotation(float inRotation) {
		rotation = inRotation;
		recomputeWorldTransform = true;
	}
#pragma endregion
		
	void ComputeWorldTransform();
	inline const Matrix4& GetWorldTransform() const { return worldTransform; }

	Vector2 GetForward() const { return Vector2(Math::Cos(rotation), Math::Sin(rotation)); }

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

	Vector2 position;
	float scale;
	float rotation;

	Matrix4 worldTransform;
	bool recomputeWorldTransform;
};