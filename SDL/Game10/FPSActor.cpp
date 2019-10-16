#include "FPSActor.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "FPSCamera.h"
#include "BoxComponent.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "PlaneActor.h"
#include "BallActor.h"

FPSActor::FPSActor(Game* inGame)
	: Actor(inGame),
	camera(new FPSCamera(this)),
	box(new BoxComponent(this)),
	mesh(nullptr),
	move(new MoveComponent(this)),
	audio(new AudioComponent(this)),
	speed(),
	attackDelay(0.1f),
	curAttackDelay(0.0f),
	fpsModel(new Actor(inGame)),
	footstep(),
	lastFootstep(0.0f) {

	fpsModel->SetScale(0.75f);
	mesh = new MeshComponent(fpsModel);
	mesh->SetMesh(inGame->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));

	const AABB aabb{
		Vector3{ -25.0f, -25.0f, -87.5f },
		Vector3{ 25.0f, 25.0f, 87.5f }
	};

	box->SetObjectBox(aabb);
	box->SetShouldRotate(false);

	footstep = audio->PlayEvent("event:/Footstep");
	footstep.SetPaused(true);
}

void FPSActor::ActorInput(const InputState& inputState) {
	Actor::ActorInput(inputState);

	const auto useController = inputState.controllers.size() > 0;
	speed = Vector2::Zero;

	if (useController) {
		speed = inputState.controllers[0].GetLeftStick();
		speed *= 400.0f;

		const auto tmp = speed.x;
		speed.x = speed.y;
		speed.y = tmp;
	}
	else {
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_W))
			speed.x += 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_S))
			speed.x -= 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_A))
			speed.y -= 400.0f;
		if (inputState.keyboard.GetKeyValue(SDL_SCANCODE_D))
			speed.y += 400.0f;
	}

	move->SetForwardSpeed(speed.x);
	move->SetStrafeSpeed(speed.y);

	const Vector2 MaxAngularSpeed{ Math::Pi * 8, Math::Pi * 8 };

	auto angularSpeed = MaxAngularSpeed;

	if (useController) {
		angularSpeed *= inputState.controllers[0].GetRightStick() * 0.25f;
		angularSpeed.y *= -1.0f;
	}
	else
		angularSpeed *= inputState.mouse.GetPosition() / 500.0f;

	move->SetAngularSpeed(angularSpeed.x);
	camera->SetPitchSpeed(angularSpeed.y);

	if ((curAttackDelay <= 0.0f)) {
		if (inputState.controllers.size() > 0) {
			if (inputState.controllers[0].GetButtonValue(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
				curAttackDelay = attackDelay;
				Shoot();
			}
		}
		else {
			if (inputState.mouse.GetButtonValue(SDL_BUTTON_LEFT)) {
				curAttackDelay = attackDelay;
				Shoot();
			}
		}
	}
}

void FPSActor::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);

	FixCollisions();

	lastFootstep -= deltaTime;
	if (speed.LengthSquared() > 0.0f && lastFootstep <= 0.0f) {
		footstep.SetPaused(false);
		footstep.Restart();
		lastFootstep = 0.5f;
	}

	const Vector3 ModelOffset{ 10.0f, 10.0f, -10.0f };
	auto modelPos = GetPosition();
	modelPos += GetForward() * ModelOffset.x;
	modelPos += GetRight() * ModelOffset.y;
	modelPos.z += ModelOffset.z;
	fpsModel->SetPosition(modelPos);

	const auto rot = Quaternion::Concatenate(GetRotation(),
		Quaternion{ GetRight(), camera->GetPitch() });
	fpsModel->SetRotation(rot);

	if (curAttackDelay > 0.0f)
		curAttackDelay -= deltaTime;
}

void FPSActor::SetFootstepSurface(const float value) {
	footstep.SetPaused(true);
	footstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(const bool visible) {
	mesh->SetVisible(visible);
}

void FPSActor::Shoot() {
	const auto renderer = GetGame()->GetRenderer();
	const auto start = renderer->Unproject(Vector3::Zero);
	const auto end = renderer->Unproject(Vector3{ 0.0f, 0.0f, 0.9f });
	const auto dir = Vector3::Normalize(end - start);

	const auto ball = new BallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.0f);
	ball->RotateToNewForward(dir);
	audio->PlayEvent("event:/Shot");
}

void FPSActor::FixCollisions() {
	ComputeWorldTransform();

	const auto& playerBox = box->GetWorldBox();
	auto pos = GetPosition();

	for (auto plane : GetGame()->GetPlanes()) {
		const auto& planeBox = plane->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox)) {
			const auto dx1 = planeBox.max.x - playerBox.min.x;
			const auto dx2 = planeBox.min.x - playerBox.max.x;
			const auto dy1 = planeBox.max.y - playerBox.min.y;
			const auto dy2 = planeBox.min.y - playerBox.max.y;
			const auto dz1 = planeBox.max.z - playerBox.min.z;
			const auto dz2 = planeBox.min.z - playerBox.max.z;

			const auto dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
			const auto dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
			const auto dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

			if (Math::Abs(dx) <= Math::Abs(dy)) {
				if (Math::Abs(dx) <= Math::Abs(dz))
					pos.x += dx;
				else
					pos.z += dz;
			}
			else {
				if (Math::Abs(dy) <= Math::Abs(dz))
					pos.y += dy;
				else
					pos.z += dz;
			}

			SetPosition(pos);
			box->OnUpdateWorldTransform();
		}
	}
}