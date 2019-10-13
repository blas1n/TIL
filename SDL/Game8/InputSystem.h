#pragma once

#include <SDL/SDL_scancode.h>
#include <SDL/SDL_gamecontroller.h>
#include <SDL/SDL_mouse.h>
#include "Math.h"

enum class ButtonState {
	ENone,
	EPressed,
	EReleased,
	EHeld
};

class KeyboardState {
public:
	bool GetKeyValue(SDL_Scancode keyCode) const;
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	friend class InputSystem;

	const Uint8* currState;
	Uint8 prevState[SDL_NUM_SCANCODES];
};

class MouseState {
public:
	inline const Vector2& GetPosition() const { return mousePos; }
	inline const Vector2& GetScrollWheel() const { return scrollWheel; }

	inline bool IsRelative() const { return isRelative; }

	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;

private:
	friend class InputSystem;

	Vector2 mousePos;
	Vector2 scrollWheel;

	Uint32 currButtons;
	Uint32 prevButtons;

	bool isRelative;
};

class ControllerState {
public:
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	inline Vector2 GetLeftStick() const { return leftStick; }
	inline Vector2 GetRightStick() const { return rightStick; }

	inline float GetLeftTrigger() const { return leftTrigger; }
	inline float GetRightTrigger() const { return rightTrigger; }

	inline bool IsConnected() const { return isConnected; }

private:
	friend class InputSystem;

	Uint8 currButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 prevButtons[SDL_CONTROLLER_BUTTON_MAX];

	Vector2 leftStick;
	Vector2 rightStick;

	float leftTrigger;
	float rightTrigger;

	bool isConnected;
};

struct InputState {
	KeyboardState keyboard;
	MouseState mouse;
	ControllerState controller;
};

class InputSystem {
public:
	bool Initialize();
	void Shutdown();

	void PrepareForUpdate();
	void Update();

	void ProcessEvent(union SDL_Event& event);

	inline const InputState& GetState() const { return state; }

	void SetRelativeMouseMode(bool value);

private:
	float Filter1D(int input);
	Vector2 Filter2D(int inputX, int inputY);

	InputState state;
	SDL_GameController* controller;
};