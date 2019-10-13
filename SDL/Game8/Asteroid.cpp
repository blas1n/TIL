#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game), circle(nullptr) {

	const auto halfScreen = Vector2{ 1024.0f, 768.0f };
	const auto randPos = Random::GetVector(-halfScreen, halfScreen);
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::Pi * 2));

	const auto sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	const auto mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	circle = new CircleComponent(this);
	circle->SetRadius(40.0f);

	game->AddAsteroid(this);
}

Asteroid::~Asteroid() {
	GetGame()->RemoveAsteroid(this);
}

void Asteroid::UpdateActor(float deltaTime) {
	auto pos = GetPosition();

	if (pos.x < -512.0f) { pos.x = 510.0f; }
	else if (pos.x > 512.0f) { pos.x = -510.0f; }

	if (pos.y < -384.0f) { pos.y = 382.0f; }
	else if (pos.y > 384.0f) { pos.y = -382.0f; }

	SetPosition(pos);
}