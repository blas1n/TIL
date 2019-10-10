#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game)
	: Actor(game), deathTimer(1.0f) {

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.0f);

	circle = new CircleComponent(this);
	circle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime) {
	if ((deathTimer -= deltaTime) <= 0.0f)
		SetState(State::EDead);
	else {
		for (auto ast : GetGame()->GetAsteroids()) {
			if (Intersect(*circle, *(ast->GetCircle()))) {
				SetState(State::EDead);
				ast->SetState(State::EDead);
				break;
			}
		}
	}
}