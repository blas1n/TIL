#include "InputClass.h"

bool InputClass::IsKeyDown(UINT key) {
	return m_keys[key];
}

void InputClass::KeyDown(UINT key) {
	m_keys[key] = true;
}

void InputClass::KeyUp(UINT key) {
	m_keys[key] = false;
}