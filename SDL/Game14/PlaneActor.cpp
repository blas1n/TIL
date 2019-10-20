#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* inGame)
	: Actor(inGame) {

	SetScale(10.0f);
	auto mc = new MeshComponent{ this };
	auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);

	const auto bc = new BoxComponent{ this };
	bc->SetObjectBox(mesh->GetBox());
}