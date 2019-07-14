
#ifndef __WINPROC_H__
#define __WINPROC_H__

#include <Windows.h>
#include <string>

class WinProc : private WNDCLASS {
	typedef std::basic_string<TCHAR> str;
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
public:
	WinProc(HINSTANCE hIn);
	virtual ~WinProc();
	str GetName() const;
};

#endif