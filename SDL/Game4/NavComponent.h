#pragma once

#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent {
public:
	NavComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	void StartPath(const class Tile* start);
	void TurnTo(const Vector2& pos);

private:
	const class Tile* nextNode;
};
