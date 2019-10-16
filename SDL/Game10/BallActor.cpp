#include "BallActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BallMove.h"
#include "AudioComponent.h"

BallActor::BallActor(Game* inGame)
	: Actor(inGame),
	audio(new AudioComponent(this)),
	move(new BallMove(this)),
	lifeSpan(2.0f) {

	const auto mc = new MeshComponent{ this };
	const auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
	mc->SetMesh(mesh);
	move->SetForwardSpeed(1500.0f);
}

void BallActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	if ((lifeSpan -= deltaTime) < 0.0f)
		SetState(State::EDead);
}

void BallActor::SetPlayer(Actor* player) {
	move->SetPlayer(player);
}

void BallActor::HitTarget() {
	audio->PlayEvent("event:/Ding");
}