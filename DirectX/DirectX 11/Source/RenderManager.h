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
	[[nodiscard]] bool RenderToTexture();
	[[nodiscard]] bool RenderScene();

private:
	class D3DManager* d3d = nullptr;
	struct DirectionalLight* light = nullptr;

	class LightShader* lightShader = nullptr;
	class TextureShader* textureShader = nullptr;

	class Camera* camera = nullptr;
	class Model* model = nullptr;

	class RenderTexture* renderTexture = nullptr;
	class DebugWindow* debugWindow = nullptr;
};
