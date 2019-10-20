#include "TargetActor.h"
#include "Game.h"
#include "Renderer.h"
#include "TargetComponent.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

TargetActor::TargetActor(Game* inGame)
	: Actor(inGame) {

	SetRotation(Quaternion{ Vector3::UnitZ, Math::Pi });
	const auto mc = new MeshComponent{ this };
	const auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);
	const auto bc = new BoxComponent{ this };
	bc->SetObjectBox(mesh->GetBox());	
	new TargetComponent{ this };
}