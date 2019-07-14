#ifndef __MYAPP_H__
#define __MYAPP_H__

#include <Windows.h>
#include "Window.h"
#include "WinProc.h"

class MyApp {
private:
	WinProc * m_pClass = nullptr;
	Window* m_pWinMain = nullptr;
public:
	MyApp(HINSTANCE hIn);
	~MyApp();
	void Run();
};

#endif