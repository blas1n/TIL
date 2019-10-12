#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	: state(State::EActive),
	components(),
	game(game),
	position(Vector2::Zero),
	scale(1.0f),
	rotation(0.0f),
	worldTransform(),
	recomputeWorldTransform(true) {

	game->AddActor(this);
}

Actor::~Actor() {
	game->RemoveActor(this);

	while (!components.empty())
		delete components.back();
}

void Actor::ProcessInput(const uint8_t* keyState) {
	if (state == State::EActive) {
		for (auto component : components)
			component->ProcessInput(keyState);

		ActorInput(keyState);
	}
}

void Actor::Update(const float deltaTime) {
	if (state == State::EActive) {
		ComputeWorldTransform();
		for (auto component : components)
			component->Update(deltaTime);

		UpdateActor(deltaTime);
		ComputeWorldTransform();
	}
}

void Actor::ComputeWorldTransform() {
	if (recomputeWorldTransform) {
		recomputeWorldTransform = false;

		worldTransform = Matrix4::CreateScale(scale);
		worldTransform *= Matrix4::CreateRotationZ(rotation);
		worldTransform *= Matrix4::CreateTranslation(Vector3{ position.x, position.y, 0.0f });

		for (auto comp : components)
			comp->OnUpdateWorldTransform();
	}
}

void Actor::AddComponent(Component* component) {
	int myOrder = component->GetUpdateOrder();

	auto iter = components.begin();
	for (; iter != components.end(); ++iter)
		if (myOrder < (*iter)->GetUpdateOrder())
			break;

	components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(components.begin(), components.end(), component);
	if (iter != components.end())
		components.erase(iter);
}