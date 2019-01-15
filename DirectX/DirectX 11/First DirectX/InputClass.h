#ifndef __INPUTCLASS_H__
#define __INPUTCLASS_H__

#include <Windows.h>

class InputClass {
private:
	bool m_keys[256];
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Init();
	void KeyDown(UINT);
	void KeyUp(UINT);
	bool IsKeyDown(UINT);
};

#endif