#include "CameraActor.h"
#include <SDL/SDL_scancode.h>
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "Renderer.h"
#include "Game.h"

CameraActor::CameraActor(Game* game)
	: Actor(game),
	move(new MoveComponent{ this }),
	audio(new AudioComponent{ this }),
	footstep(),
	lastFootstep(0.0f) {

	footstep = audio->PlayEvent("event:/Footstep");
	footstep.SetPaused(true);
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

	lastFootstep -= deltaTime;
	if (!Math::NearZero(move->GetForwardSpeed()) && lastFootstep <= 0.0f) {
		footstep.SetPaused(false);
		footstep.Restart();
		lastFootstep = 0.5f;
	}

	const auto cameraPos = GetPosition();
	const auto target = cameraPos + GetForward() * 100.0f;
	const auto up = Vector3::UnitZ;

	const auto view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
	GetGame()->GetAudioSystem()->SetListener(view);
}

void CameraActor::SetFootstepSurface(float value) {
	footstep.SetPaused(true);
	footstep.SetParameter("Surface", value);
}