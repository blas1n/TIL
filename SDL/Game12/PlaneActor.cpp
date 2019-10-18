#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* inGame)
	: Actor(inGame), box(nullptr) {

	SetScale(10.0f);
	auto mc = new MeshComponent{ this };
	auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);

	box = new BoxComponent{ this };
	box->SetObjectBox(mesh->GetBox());

	inGame->AddPlane(this);
}

PlaneActor::~PlaneActor() {
	GetGame()->RemoveActor(this);
}