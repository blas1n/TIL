#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* inGame)
	: state(State::EActive),
	components(),
	game(inGame),
	position(Vector3::Zero),
	rotation(Quaternion::Identity),
	scale(1.0f),
	worldTransform(),
	recomputeWorldTransform(true) {

	game->AddActor(this);
}

Actor::~Actor() {
	game->RemoveActor(this);

	while (!components.empty())
		delete components.back();
}

void Actor::ProcessInput(const InputState& inputState) {
	if (state == State::EActive) {
		for (auto component : components)
			component->ProcessInput(inputState);

		ActorInput(inputState);
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
		worldTransform *= Matrix4::CreateFromQuaternion(rotation);
		worldTransform *= Matrix4::CreateTranslation(position);

		for (auto comp : components)
			comp->OnUpdateWorldTransform();
	}
}

void Actor::RotateToNewForward(const Vector3& forward) {
	const auto dot = Vector3::Dot(Vector3::UnitX, forward);
	
	if (dot > 0.9999f)
		SetRotation(Quaternion::Identity);
	else if (dot < -0.9999f)
		SetRotation(Quaternion{ Vector3::UnitZ, Math::Pi });
	else {
		auto axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalized();
		SetRotation(Quaternion{ axis, Math::Acos(dot) });
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