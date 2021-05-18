#pragma once

#include "stdafx.h"
#include <tuple>

class SystemManager final
{
public:
	SystemManager() = default;
	
	SystemManager(const SystemManager&) = delete;
	SystemManager(SystemManager&&) = delete;
	
	SystemManager& operator=(const SystemManager&) = delete;
	SystemManager& operator=(SystemManager&&) = delete;
	
	~SystemManager() = default;

	[[nodiscard]] bool Init();
	[[nodiscard]] int Run();
	void Release();

private:
	bool Frame();
	
	POINT InitWindows();
	void ReleaseWindows();

private:
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCTSTR appName;

	HINSTANCE hInstance;
	HWND hWnd;

	class RenderManager* render;
	class InputManager* input;
};