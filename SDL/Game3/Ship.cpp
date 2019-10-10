#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
	:Actor(game), laserCooldown(0.0f) {

	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	InputComponent* ic = new InputComponent(this);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::Pi * 2);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && laserCooldown <= 0.0f) {
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		laserCooldown = 0.5f;
	}
}

void Ship::UpdateActor(float deltaTime) {
	laserCooldown -= deltaTime;

	auto pos = GetPosition();

	if (pos.x < 20.0f) { pos.x = 20.0f; }
	else if (pos.x > 1004.0f) { pos.x = 1004.0f; }

	if (pos.y < 20.0f) { pos.y = 20.0f; }
	else if (pos.y > 748.0f) { pos.y = 748.0f; }

	SetPosition(pos);
}