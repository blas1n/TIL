#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "CircleComponent.h"
#include <algorithm>

Enemy::Enemy(Game* game)
	: Actor(game) {

	game->GetEnemies().emplace_back(this);
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());

	auto sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	
	auto nc = new NavComponent(this);
	nc->SetForwardSpeed(150.0f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());

	circle = new CircleComponent(this);
	circle->SetRadius(25.0f);
}

Enemy::~Enemy() {
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(), this);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(const float deltaTime) {
	Actor::UpdateActor(deltaTime);

	const auto diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();

	if (Math::NearZero(diff.Length(), 10.0f))
		SetState(State::EDead);
}
