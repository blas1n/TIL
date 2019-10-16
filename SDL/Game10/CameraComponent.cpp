#include "CameraComponent.h"
#include "AudioSystem.h"
#include "Renderer.h"
#include "Actor.h"
#include "Game.h"

CameraComponent::CameraComponent(Actor* inOwner, const int inUpdateOrder /*= 200*/)
	: Component(inOwner, inUpdateOrder) {}

void CameraComponent::SetViewMatrix(const Matrix4& view) {
	const auto game = owner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
	game->GetAudioSystem()->SetListener(view);
}