#include "Ship.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "InputComponent.h"
#include "InputSystem.h"
#include "Asteroid.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
	:Actor(game),
	circle(nullptr),
	velocityDir(),
	rotationDir(),
	speed(400.0f),
	laserCooldown(0.0f) {

	auto sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	circle = new CircleComponent(this);
	circle->SetRadius(30.0f);

	auto ic = new InputComponent(this);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::Pi * 2);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
}

void Ship::ActorInput(const InputState& state) {
	const auto isClick =
		state.mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::EPressed;

	if (isClick && laserCooldown <= 0.0f) {
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		laserCooldown = 0.5f;
	}

	if (state.controllers.size() > 0) {
		velocityDir = state.controllers[0].GetLeftStick();
		if (!Math::NearZero(state.controllers[0].GetRightStick().LengthSquared()))
			rotationDir = state.controllers[0].GetRightStick();
	}
}

void Ship::UpdateActor(const float deltaTime) {
	laserCooldown -= deltaTime;

	auto pos = GetPosition() + velocityDir * speed * deltaTime;

	if (pos.x < -492.0f) { pos.x = -492.0f; }
	else if (pos.x > 492.0f) { pos.x = 492.0f; }

	if (pos.y < -373.0f) { pos.y = -373.0f; }
	else if (pos.y > 373.0f) { pos.y = 373.0f; }

	SetPosition(pos);

	if (!Math::NearZero(rotationDir.LengthSquared())) {
		auto angle = Math::Atan2(rotationDir.y, rotationDir.x);
		SetRotation(angle);
	}

	for (auto ast : GetGame()->GetAsteroids()) {
		if (Intersect(*circle, *(ast->GetCircle()))) {
			GetGame()->DeadShip();
			break;
		}
	}
}