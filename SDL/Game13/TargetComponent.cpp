#include "TargetComponent.h"
#include "Actor.h"
#include "Game.h"
#include "HUD.h"

TargetComponent::TargetComponent(Actor* inOwner)
	: Component(inOwner) {

	owner->GetGame()->GetHUD()->AddTargetComponent(this);
}

TargetComponent::~TargetComponent() {
	owner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}