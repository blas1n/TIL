#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner)
	: MoveComponent(owner),
	forwardKey(0),
	backKey(0),
	clockwiseKey(0),
	counterClockwiseKey(0) {}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	float forwardSpeed = 0.0f;

	if (keyState[forwardKey])
		forwardSpeed += forwardSpeed;
	if (keyState[backKey])
		forwardSpeed -= forwardSpeed;

	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.0f;

	if (keyState[clockwiseKey])
		angularSpeed += angularSpeed;
	if (keyState[counterClockwiseKey])
		angularSpeed -= angularSpeed;

	SetAngularSpeed(angularSpeed);
}