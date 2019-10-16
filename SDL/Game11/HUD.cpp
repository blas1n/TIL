#include "HUD.h"
#include <algorithm>
#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FPSActor.h"
#include "TargetComponent.h"

HUD::HUD(Game* inGame)
	: UiScreen(inGame),
	healthBar(nullptr),
	radar(nullptr),
	crosshair(nullptr),
	crosshairEnemy(nullptr),
	blipTex(nullptr),
	radarArrow(nullptr),
	targets(),
	blips(),
	radarRange(2000.0f),
	radarRadius(92.0f),
	targetEnemy(false) {

	const auto r = game->GetRenderer();
	healthBar = r->GetTexture("Assets/HealthBar.png");
	radar = r->GetTexture("Assets/Radar.png");
	crosshair = r->GetTexture("Assets/Crosshair.png");
	crosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	blipTex = r->GetTexture("Assets/Blip.png");
	radarArrow = r->GetTexture("Assets/RadarArrow.png");
}

void HUD::Update(const float deltaTime) {
	UiScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader) {
	const auto cross = targetEnemy ? crosshairEnemy : crosshair;
	DrawTexture(shader, cross, Vector2::Zero, 2.0f);

	const Vector2 RadarPos{ -390.0f, 275.0f };
	DrawTexture(shader, radar, RadarPos);
	
	for (const auto& blip : blips)
		DrawTexture(shader, blipTex, RadarPos + blip, 1.0f);

	DrawTexture(shader, radarArrow, RadarPos);
}

void HUD::AddTargetComponent(TargetComponent* tc) {
	targets.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc) {
	auto iter = std::find(targets.cbegin(), targets.cend(), tc);
	targets.erase(iter);
}

void HUD::UpdateCrosshair(const float deltaTime) {
	targetEnemy = false;

	const float AimDist = 5000.0f;
	Vector3 start, dir;
	game->GetRenderer()->GetScreenDirection(Vector2::Zero,start, dir);
	LineSegment l{ start, start + dir * AimDist };

	PhysWorld::CollisionInfo info;
	if (game->GetPhysWorld()->SegmentCast(l, info)) {
		for (auto target : targets) {
			if (target->GetOwner() == info.actor) {
				targetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateRadar(const float deltaTime) {
	blips.clear();

	const auto playerPos = game->GetPlayer()->GetPosition();
	const Vector2 playerPos2D{ playerPos.y, playerPos.x };

	const auto playerForward = game->GetPlayer()->GetForward();
	const Vector2 playerForward2D{ playerForward.x, playerForward.y };

	const auto angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	const auto rotMat = Matrix3::CreateRotation(angle);

	for (auto target : targets) {
		const auto targetPos = target->GetOwner()->GetPosition();
		const Vector2 actorPos2D{ targetPos.y, targetPos.x };

		const auto playerToTarget = actorPos2D - playerPos2D;

		if (playerToTarget.LengthSquared() <= (radarRange * radarRange)) {
			auto blipPos = playerToTarget;
			blipPos *= radarRadius / radarRange;
			blipPos = Vector2::Transform(blipPos, rotMat);
			blips.emplace_back(blipPos);
		}
	}
}