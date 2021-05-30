#pragma once

#include "stdafx.h"
#include <tuple>

class System final
{
public:
	System() = default;
	
	System(const System&) = delete;
	System(System&&) = delete;
	
	System& operator=(const System&) = delete;
	System& operator=(System&&) = delete;
	
	~System() = default;

	[[nodiscard]] bool Init();
	[[nodiscard]] int Run();
	void Release() noexcept;

private:
	bool Frame();
	
	SIZE InitWindows();
	void ReleaseWindows() noexcept;

private:
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCTSTR appName;

	HINSTANCE hInstance;
	HWND hWnd;

	class RenderManager* render;
	class InputManager* input;
};