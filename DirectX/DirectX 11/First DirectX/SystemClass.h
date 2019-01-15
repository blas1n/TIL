#ifndef __SYSTEMCLASS_H__
#define __SYSTEMCLASS_H__

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <tchar.h>
#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass {
private:
	LPCTSTR m_appName;
	HINSTANCE m_hIn;
	HWND m_hWnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Init();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitWindows(int&, int&);
	void ShutDownWindows();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* hApp = nullptr;

#endif