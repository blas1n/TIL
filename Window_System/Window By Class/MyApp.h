#ifndef __MYAPP_H__
#define __MYAPP_H__

#include <Windows.h>
#include "MyWndClass.h"
#include "Window.h"

class MyApp {
private:
	MyWndClass* m_pClass = nullptr;
	Window* m_pMainWindow = nullptr;
public:
	MyApp(HINSTANCE hIn);
	~MyApp();
	void Run();
};

#endif