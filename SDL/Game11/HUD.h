#pragma once

#include "UiScreen.h"

class HUD : public UiScreen {
public:
	HUD(Game* inGame);

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;

	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(TargetComponent* tc);

private:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	Texture* healthBar;
	Texture* radar;
	Texture* crosshair;
	Texture* crosshairEnemy;
	Texture* blipTex;
	Texture* radarArrow;

	std::vector<TargetComponent*> targets;
	std::vector<Vector2> blips;

	float radarRange;
	float radarRadius;

	bool targetEnemy;
};