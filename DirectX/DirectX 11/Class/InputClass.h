#pragma once

#include "../Utility//NonMovable.h"

class InputClass : public NonMovable {
private:
	using UINT = unsigned int;

public:
	bool IsKeyDown(UINT);

	void KeyDown(UINT);
	void KeyUp(UINT);

private:
	bool m_keys[256];
};