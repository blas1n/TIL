#include "FPSActor.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "AudioComponent.h"

FPSActor::FPSActor(Game* inGame)
	: Actor(inGame),
	mesh(new MeshComponent(this)),
	move(new MoveComponent(this)),
	audio(new AudioComponent(this)),
	speed(),
	footstep(),
	lastFootstep(0.0f) {

	footstep = audio->PlayEvent("event:/Footstep");
	footstep.SetPaused(true);
}

void FPSActor::ActorInput(const InputState& inputState) {
	Actor::ActorInput(inputState);

	speed = Vector2::Zero;

	if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_W))
		speed.x += 400.0f;
	if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_S))
		speed.x -= 400.0f;
	if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_D))
		speed.y += 400.0f;
	if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_A))
		speed.y -= 400.0f;

	move->SetForwardSpeed(speed.x);
	move->SetStrafeSpeed(speed.y);
}

void FPSActor::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);

	lastFootstep -= deltaTime;
	if (speed.LengthSquared() > 0.0f && lastFootstep <= 0.0f) {
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

void FPSActor::SetFootstepSurface(const float value) {
	footstep.SetPaused(true);
	footstep.SetParameter("Surface", value);
}