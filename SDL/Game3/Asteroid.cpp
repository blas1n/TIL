#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game)
	:Actor(game), circle(nullptr) {

	const auto randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
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

	if (pos.x < 0.0f) { pos.x = 1022.0f; }
	else if (pos.x > 1024.0f) { pos.x = 2.0f; }

	if (pos.y < 0.0f) { pos.y = 766.0f; }
	else if (pos.y > 768.0f) { pos.y = 2.0f; }

	SetPosition(pos);
}