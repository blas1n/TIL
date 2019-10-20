#include "HUD.h"
#include <algorithm>
#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FollowActor.h"
#include "TargetComponent.h"

HUD::HUD(Game* inGame)
	: UiScreen(inGame),
	healthBar(nullptr),
	radar(nullptr),
	blipTex(nullptr),
	radarArrow(nullptr),
	targets(),
	blips(),
	radarRange(2000.0f),
	radarRadius(92.0f) {

	const auto r = game->GetRenderer();
	healthBar = r->GetTexture("Assets/HealthBar.png");
	radar = r->GetTexture("Assets/Radar.png");
	blipTex = r->GetTexture("Assets/Blip.png");
	radarArrow = r->GetTexture("Assets/RadarArrow.png");
}

void HUD::Update(const float deltaTime) {
	UiScreen::Update(deltaTime);

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

void HUD::Draw(Shader* shader) {
	const Vector2 RadarPos{ -390.0f, 275.0f };
	DrawTexture(shader, radar, RadarPos);
	
	for (const auto& blip : blips)
		DrawTexture(shader, blipTex, RadarPos + blip, 1.0f);

	DrawTexture(shader, radarArrow, RadarPos);

	const auto mirror = game->GetRenderer()->GetMirrorTexture();
	DrawTexture(shader, mirror, Vector2{ -350.0f, -250.0f }, 1.0f, true);
}

void HUD::AddTargetComponent(TargetComponent* tc) {
	targets.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc) {
	auto iter = std::find(targets.cbegin(), targets.cend(), tc);
	targets.erase(iter);
}