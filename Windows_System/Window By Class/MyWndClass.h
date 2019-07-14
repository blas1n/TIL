#ifndef __MYWNDCLASS_H__
#define __MYWNDCLASS_H__

#include <Windows.h>
#include <string>

class MyWndClass : private WNDCLASS {
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
		WPARAM wParam, LPARAM lParam);
public:
	MyWndClass(HINSTANCE hIn);

	virtual ~MyWndClass();

	std::basic_string<TCHAR> GetName() const;
};

#endif