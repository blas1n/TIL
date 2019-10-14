#include "FollowActor.h"
#include "FollowCamera.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "Game.h"

FollowActor::FollowActor(Game* inGame)
	: Actor(inGame),
	camera(new FollowCamera(this)),
	mesh(new MeshComponent(this)),
	move(new MoveComponent(this)) {

	SetPosition(Vector3{ 0.0f, 0.0f, -100.0f });
	mesh->SetMesh(inGame->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	camera->SnapToIdeal();
}
void FollowActor::ActorInput(const InputState& inputState) {
	Actor::ActorInput(inputState);
	const auto useController = inputState.controllers.size() > 0;
	auto speed = 0.0f, angularSpeed = 0.0f;

	if (useController) {
		const auto stick = inputState.controllers[0].GetLeftStick();
		speed = stick.y * 400.0f;
		angularSpeed = stick.x * Math::Pi;
	}
	else {
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_W))
			speed += 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_S))
			speed -= 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_A))
			angularSpeed -= Math::Pi;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_D))
			angularSpeed += Math::Pi;
	}

	move->SetForwardSpeed(speed);
	move->SetAngularSpeed(angularSpeed);

	const auto dist = Math::NearZero(speed) ? 350.0f : 500.0f;
	camera->SetHorizontalDistance(dist);
}

void FollowActor::SetVisible(const bool visible) {
	mesh->SetVisible(visible);
}