#include "InputSystem.h"
#include <SDL/SDL.h>
#include <cstring>

bool KeyboardState::GetKeyValue(const SDL_Scancode keyCode) const {
	return currState[keyCode];
}

ButtonState KeyboardState::GetKeyState(const SDL_Scancode keyCode) const {
	if (prevState[keyCode]) {
		if (currState[keyCode])
			return ButtonState::EHeld;
		else
			return ButtonState::EReleased;
	}
	else {
		if (currState[keyCode])
			return ButtonState::EPressed;
		else
			return ButtonState::ENone;
	}
}

bool MouseState::GetButtonValue(const int button) const {
	return currButtons & SDL_BUTTON(button);
}

ButtonState MouseState::GetButtonState(const int button) const {
	auto mask = SDL_BUTTON(button);

	if (prevButtons & mask) {
		if (currButtons & mask)
			return ButtonState::EHeld;
		else
			return ButtonState::EReleased;
	}
	else {
		if (currButtons & mask)
			return ButtonState::EPressed;
		else
			return ButtonState::ENone;
	}
}

bool ControllerState::GetButtonValue(const SDL_GameControllerButton button) const {
	return currButtons[button];
}

ButtonState ControllerState::GetButtonState(const SDL_GameControllerButton button) const {
	if (prevButtons[button]) {
		if (currButtons[button])
			return ButtonState::EHeld;
		else
			return ButtonState::EReleased;
	}
	else {
		if (currButtons[button])
			return ButtonState::EPressed;
		else
			return ButtonState::ENone;
	}
}

bool InputSystem::Initialize() {
	state.keyboard.currState = SDL_GetKeyboardState(nullptr);
	memset(state.keyboard.prevState, 0, SDL_NUM_SCANCODES);

	state.mouse.currButtons = state.mouse.prevButtons = 0;

	controller = SDL_GameControllerOpen(0);
	state.controller.isConnected = controller != nullptr;
	memset(state.controller.currButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(state.controller.prevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Shutdown() {}

void InputSystem::PrepareForUpdate() {
	memcpy(state.keyboard.prevState,
		state.keyboard.currState,
		SDL_NUM_SCANCODES);

	state.mouse.prevButtons = state.mouse.currButtons;
	state.mouse.scrollWheel = Vector2::Zero;

	memcpy(state.controller.prevButtons,
		state.controller.currButtons,
		SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update() {
	auto x = 0, y = 0;

	if (state.mouse.isRelative)
		state.mouse.currButtons = SDL_GetRelativeMouseState(&x, &y);
	else
		state.mouse.currButtons = SDL_GetMouseState(&x, &y);

	state.mouse.mousePos.x = static_cast<float>(x);
	state.mouse.mousePos.y = static_cast<float>(y);

	for (auto i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		state.controller.currButtons[i]
			= SDL_GameControllerGetButton(controller,
				SDL_GameControllerButton(i));

	x = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_LEFTX);
	y = -SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_LEFTY);

	state.controller.leftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_RIGHTX);
	y = -SDL_GameControllerGetAxis(controller,
		SDL_CONTROLLER_AXIS_RIGHTY);

	state.controller.rightStick = Filter2D(x, y);

	state.controller.leftTrigger =
		Filter1D(SDL_GameControllerGetAxis(controller,
			SDL_CONTROLLER_AXIS_TRIGGERLEFT));

	state.controller.rightTrigger =
		Filter1D(SDL_GameControllerGetAxis(controller,
			SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
}

void InputSystem::ProcessEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_MOUSEWHEEL:
		state.mouse.scrollWheel = Vector2{
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y)
		};
		break;
	default:
		break;
	}
}

void InputSystem::SetRelativeMouseMode(const bool value) {
	SDL_SetRelativeMouseMode(value ? SDL_TRUE : SDL_FALSE);
	state.mouse.isRelative = value;
}

float InputSystem::Filter1D(const int input) {
	constexpr static auto deadZone = 250;
	constexpr static auto maxValue = 30000;

	auto ret = 0.0f;
	const auto absValue = Math::Abs(input);
	
	if (absValue > deadZone) {
		ret = static_cast<float>(absValue - deadZone) / static_cast<float>(maxValue - deadZone);
		if (input > 0) ret *= -1.0f;
		ret = Math::Clamp(ret, -1.0f, 1.0f);
	}

	return ret;
}

Vector2 InputSystem::Filter2D(const int inputX, const int inputY) {
	constexpr static auto deadZone = 8000.0f;
	constexpr static auto maxValue = 30000.0f;

	Vector2 dir{
		static_cast<float>(inputX),
		static_cast<float>(inputY)
	};

	auto length = dir.LengthSquared();

	if (length < deadZone * deadZone)
		dir = Vector2::Zero;
	else {
		length = Math::Sqrt(length);
		const auto f = Math::Clamp(
			(length - deadZone) / (maxValue - deadZone),
			0.0f, 1.0f);

		dir *= f / length;
	}

	return dir;
}