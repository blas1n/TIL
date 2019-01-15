#include "InputClass.h"

InputClass::InputClass() {}

InputClass::InputClass(const InputClass& other) {}

InputClass::~InputClass() {}

void InputClass::Init() {
	memset(m_keys, false, 256 * sizeof(bool));
}

void InputClass::KeyDown(UINT input) {
	m_keys[input] = true;
}

void InputClass::KeyUp(UINT input) {
	m_keys[input] = false;
}

bool InputClass::IsKeyDown(UINT key) {
	return m_keys[key];
}