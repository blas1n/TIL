#include "Actor.h"
#include "Game.h"
#include "JsonHelper.h"
#include "LevelLoader.h"
#include <algorithm>

std::map<Actor::TypeId, const char*> Actor::TypeNames {
	{ Actor::TypeId::TActor, "Actor" },
	{ Actor::TypeId::TFollowActor, "FollowActor" },
	{ Actor::TypeId::TPlaneActor, "PlaneActor" },
	{ Actor::TypeId::TTargetActor, "TargetActor" },
};

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

void Actor::LoadProperties(const rapidjson::Value& inObj) {
	std::string state;
	if (JsonHelper::GetString(inObj, "state", state)) {
		if (state == "active")
			SetState(State::EActive);
		else if (state == "paused")
			SetState(State::EPaused);
		else if (state == "dead")
			SetState(State::EDead);
	}

	JsonHelper::GetVector3(inObj, "position", position);
	JsonHelper::GetQuaternion(inObj, "rotation", rotation);
	JsonHelper::GetFloat(inObj, "scale", scale);
	ComputeWorldTransform();
}

void Actor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
	std::string stateStr = "active";
	if (state == State::EPaused)
		stateStr = "paused";
	else if (state == State::EDead)
		stateStr = "dead";

	JsonHelper::AddString(alloc, inObj, "state", stateStr);
	JsonHelper::AddVector3(alloc, inObj, "position", position);
	JsonHelper::AddQuaternion(alloc, inObj, "rotation", rotation);
	JsonHelper::AddFloat(alloc, inObj, "scale", scale);
}
