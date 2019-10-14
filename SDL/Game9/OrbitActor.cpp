#include "OrbitActor.h"
#include "OrbitCamera.h"
#include "MeshComponent.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "Game.h"

OrbitActor::OrbitActor(Game* inGame)
	: Actor(inGame),
	camera(new OrbitCamera(this)),
	mesh(new MeshComponent(this)) {

	SetPosition(Vector3{ -150.0f, -150.0f, -100.0f });
	mesh->SetMesh(inGame->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
}

void OrbitActor::ActorInput(const InputState& inputState) {
	Actor::ActorInput(inputState);
	const auto MaxOrbitSpeed = Math::Pi * 8.0f;
	auto speed = Vector2::Zero;

	if (inputState.controllers.size() > 0) {
		if (inputState.controllers[0].GetButtonValue(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
			speed = inputState.controllers[0].GetLeftStick() * MaxOrbitSpeed;
	}
	else {
		if (inputState.mouse.GetButtonValue(SDL_BUTTON_RIGHT))
			speed = inputState.mouse.GetPosition() / 500.0f * MaxOrbitSpeed;
	}

	camera->SetYawSpeed(speed.x);
	camera->SetPitchSpeed(speed.y);
}

void OrbitActor::SetVisible(const bool visible) {
	mesh->SetVisible(visible);
}