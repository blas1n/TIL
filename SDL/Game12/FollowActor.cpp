#include "FollowActor.h"
#include "FollowCamera.h"
#include "SkeletalMeshComponent.h"
#include "MoveComponent.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "Game.h"

FollowActor::FollowActor(Game* inGame)
	: Actor(inGame),
	move(new MoveComponent(this)),
	camera(new FollowCamera(this)),
	mesh(new SkeletalMeshComponent(this)),
	isMoving(false) {

	mesh->SetMesh(inGame->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
	mesh->SetSkeleton(inGame->GetRenderer()->GetSkeleton("Assets/CatWarrior.gpskel"));
	mesh->PlayAnimation(inGame->GetRenderer()->GetAnimation("Assets/CatActionIdle.gpanim"));

	SetPosition(Vector3{ 0.0f, 0.0f, -100.0f });
	camera->SnapToIdeal();
}

void FollowActor::ActorInput(const InputState& inputState) {
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

	const auto nowMoving = !Math::NearZero(speed);

	if (isMoving) {
		if (!nowMoving)
			mesh->PlayAnimation(GetGame()->GetRenderer()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}
	else if (nowMoving) 
		mesh->PlayAnimation(GetGame()->GetRenderer()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);

	isMoving = nowMoving;
}

void FollowActor::SetVisible(const bool visible) {
	mesh->SetVisible(visible);
}