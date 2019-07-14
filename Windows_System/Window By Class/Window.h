#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Windows.h>
#include <string>

class Window
{
private:
	HWND m_hWnd = nullptr;
public:
	Window(const std::basic_string<TCHAR> &className,
		const std::basic_string<TCHAR> &windowName,
		int nX, int nY, int nWidth, int nHeight,
		HINSTANCE hIn, void* pData = nullptr);

	Window(const std::basic_string<TCHAR> &className,
		const std::basic_string<TCHAR> &windowName,
		HINSTANCE hIn, void* pData = nullptr);

	virtual ~Window();

	void Show(bool wantShow);

	HWND GetHwnd() const;
};

#endif