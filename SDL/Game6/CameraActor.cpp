#include "CameraActor.h"
#include <SDL/SDL_scancode.h>
#include "MoveComponent.h"
#include "Renderer.h"
#include "Game.h"

CameraActor::CameraActor(Game* game)
	: Actor(game) {

	move = new MoveComponent(this);
}

void CameraActor::ActorInput(const uint8_t* keys) {
	auto forwardSpeed = 0.0f;
	auto angularSpeed = 0.0f;

	if (keys[SDL_SCANCODE_W])
		forwardSpeed += 300.0f;
	if (keys[SDL_SCANCODE_S])
		forwardSpeed -= 300.0f;
	if (keys[SDL_SCANCODE_A])
		angularSpeed -= Math::Pi * 2;
	if (keys[SDL_SCANCODE_D])
		angularSpeed += Math::Pi * 2;

	move->SetForwardSpeed(forwardSpeed);
	move->SetAngularSpeed(angularSpeed);
}

void CameraActor::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);

	const auto cameraPos = GetPosition();
	const auto target = cameraPos + GetForward() * 100.0f;
	const auto up = Vector3::UnitZ;

	const auto view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}