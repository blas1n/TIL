#pragma once

#include "stdafx.h"

class InputManager final
{
public:
	[[nodiscard]] bool Initialize(HINSTANCE hInst, HWND hWnd, SIZE screenSize);

	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;

	InputManager& operator=(const InputManager&) = delete;
	InputManager& operator=(InputManager&&) = delete;

	~InputManager() = default;

	[[nodiscard]] bool Frame();
	void Release() noexcept;

	bool IsPressed(unsigned int key) const noexcept { return keyState[key] & 0x80; }
	POINT GetMouseLocation() const noexcept;

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	struct InputImpl* impl = nullptr;

	unsigned char keyState[256];
	POINT size, mousePos;
};