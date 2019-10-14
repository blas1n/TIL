#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"

PlaneActor::PlaneActor(Game* inGame)
	: Actor(inGame) {

	SetScale(10.0f);
	auto mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}