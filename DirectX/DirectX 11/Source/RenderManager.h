#pragma once

#include "D3DManager.h"
#include <memory>
#include <tuple>
#include "ColorShaderClass.h"
#include "CameraClass.h"
#include "ModelClass.h"

constexpr static  float SCREEN_DEPTH = 1000.0f;
constexpr static  float SCREEN_NEAR = 0.1f;
constexpr static bool VSYNC_ENABLED = true;
constexpr static bool FULL_SCREEN = true;

class RenderManager final
{
public:
	bool Init(std::tuple<int, int>, HWND);
	void Release();

	bool Frame();

private:
	bool Render();

private:
	std::unique_ptr<D3DManager> m_D3D;
	std::unique_ptr<CameraClass> m_camera;
	std::unique_ptr<ModelClass> m_model;
	std::unique_ptr<ColorShaderClass> m_colorShader;
};