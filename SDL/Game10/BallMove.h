#pragma once

#include "MoveComponent.h"

class BallMove : public MoveComponent {
public:
	BallMove(Actor* inOwner);

	void Update(float deltaTime) override;
	inline void SetPlayer(Actor* inPlayer) { player = inPlayer; }

protected:
	Actor* player;
};

