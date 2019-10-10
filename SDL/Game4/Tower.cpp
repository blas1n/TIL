#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(Game* game)
	: Actor(game) {

	auto sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));
	
	move = new MoveComponent(this);
	
	nextAttack = AttackTime;
}

void Tower::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);
	
	if ((nextAttack -= deltaTime) <= 0.0f) {
		auto enemy = GetGame()->GetNearestEnemy(GetPosition());
		if (enemy) {
			const auto dir = enemy->GetPosition() - GetPosition();

			if (dir.Length() < AttackRange) {
				SetRotation(Math::Atan2(-dir.y, dir.x));

				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		nextAttack += AttackTime;
	}
}
