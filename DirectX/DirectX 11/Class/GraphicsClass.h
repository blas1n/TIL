#pragma once

#include "../Utility//NonMovable.h"
#include "D3dClass.h"
#include "ColorShaderClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include <memory>
#include <tuple>

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass : public NonMovable {
public:
	bool Init(std::tuple<int, int>, HWND);
	void Release();

	bool Frame();

private:
	bool Render();

private:
	std::unique_ptr<D3dClass> m_D3D;
	std::unique_ptr<CameraClass> m_camera;
	std::unique_ptr<ModelClass> m_model;
	std::unique_ptr<ColorShaderClass> m_colorShader;
};