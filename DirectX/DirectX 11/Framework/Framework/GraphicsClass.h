#pragma once

#include "NonMovable.h"
#include <Windows.h>
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
};