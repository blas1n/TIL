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

	void ProcessInput(const uint8_t* keyState);
	void Update(float deltaTime);
	
#pragma region GetterAndSetter
	const Vector2& GetPosition() const { return position; }
	void SetPosition(const Vector2& pos) { position = pos; }
	float GetScale() const { return scale; }
	void SetScale(float scale) { scale = scale; }
	float GetRotation() const { return rotation; }
	void SetRotation(float rotation) { rotation = rotation; }
#pragma endregion
		
	Vector2 GetForward() const { return Vector2(Math::Cos(rotation), -Math::Sin(rotation)); }

	State GetState() const { return state; }
	void SetState(State state) { state = state; }

	class Game* GetGame() { return game; }

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

protected:
	virtual void ActorInput(const uint8_t* keyState) {}
	virtual void UpdateActor(float deltaTime) {}

private:
	State state;

	Vector2 position;
	float scale;
	float rotation;

	std::vector<Component*> components;
	class Game* game;
};