#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Ship::Ship(Game* game)
	: Actor(game),
	rightSpeed(0.0f),
	downSpeed(0.0f) {

	auto asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
	};

	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);

	Vector2 pos = GetPosition();
	pos.x += rightSpeed * deltaTime;
	pos.y += downSpeed * deltaTime;

	if (pos.x < 25.0f)
		pos.x = 25.0f;
	else if (pos.x > 500.0f)
		pos.x = 500.0f;
	if (pos.y < 25.0f)
		pos.y = 25.0f;
	else if (pos.y > 743.0f)
		pos.y = 743.0f;

	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state)
{
	rightSpeed = 0.0f;
	downSpeed = 0.0f;

	if (state[SDL_SCANCODE_D])
	{
		rightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		rightSpeed -= 250.0f;
	}

	if (state[SDL_SCANCODE_S])
	{
		downSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		downSpeed -= 300.0f;
	}
}
