#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Windows.h>
#include <string>

class Window {
	typedef std::basic_string<TCHAR> str;
private:
	HWND m_hWnd = nullptr;
public:
	Window(const str& className, const str& windowName,
		int nX, int nY, int nWidth, int nHeight,
		HINSTANCE hIn, void* pData = nullptr);

	Window(const str& className, const str& windowName,
		HINSTANCE hIn, void* pData = nullptr);

	virtual ~Window();

	void Show(bool wantShow);

	HWND GetHwnd() const;
};

#endif