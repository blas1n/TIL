#include "Bullet.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Enemy.h"

Bullet::Bullet(Game* game)
	: Actor(game) {

	auto sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Projectile.png"));
	
	auto mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.0f);
	
	circle = new CircleComponent(this);
	circle->SetRadius(5.0f);
	
	liveTime = 1.0f;
}

void Bullet::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);
	
	for (auto enemy : GetGame()->GetEnemies()) {
		if (Intersect(*circle, *(enemy->GetCircle()))) {
			enemy->SetState(State::EDead);
			SetState(State::EDead);
			break;
		}
	}

	if ((liveTime -= deltaTime) <= 0.0f)
		SetState(State::EDead);
}
