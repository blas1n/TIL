#include "InputComponent.h"
#include "InputSystem.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner)
	: MoveComponent(owner),
	maxForwardSpeed(0.0f),
	maxAngularSpeed(0.0f),
	forwardKey(0),
	backKey(0),
	clockwiseKey(0),
	counterClockwiseKey(0) {}

void InputComponent::ProcessInput(const InputState& state) {
	float forwardSpeed = 0.0f;

	if (state.keyboard.GetKeyValue(SDL_Scancode(forwardKey)))
		forwardSpeed += maxForwardSpeed;
	if (state.keyboard.GetKeyValue(SDL_Scancode(backKey)))
		forwardSpeed -= maxForwardSpeed;

	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.0f;

	if (state.keyboard.GetKeyValue(SDL_Scancode(clockwiseKey)))
		angularSpeed += maxAngularSpeed;
	if (state.keyboard.GetKeyValue(SDL_Scancode(counterClockwiseKey)))
		angularSpeed -= maxAngularSpeed;

	SetAngularSpeed(angularSpeed);
}