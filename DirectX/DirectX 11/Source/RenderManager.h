#pragma once

#include "stdafx.h"

constexpr static float SCREEN_FAR = 1000.0f;
constexpr static float SCREEN_NEAR = 0.1f;
constexpr static bool VSYNC_ENABLED = true;
constexpr static bool FULL_SCREEN = false;

class RenderManager final
{
public:
	[[nodiscard]] bool Initialize(HWND hWnd, SIZE size);
	[[nodiscard]] bool Frame();
	void Release() noexcept;

private:
	class D3DManager* d3d = nullptr;
	class AlphaMapShader* shader = nullptr;
	class Camera* camera = nullptr;
	class Model* model = nullptr;
};
