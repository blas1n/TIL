#pragma once

#include "stdafx.h"

class RenderManager final
{
public:
	[[nodiscard]] bool Initialize(HWND hWnd, POINT size);
	[[nodiscard]] bool Frame();
	[[nodiscard]] void Release();

private:
	class D3DManager* d3d;
};