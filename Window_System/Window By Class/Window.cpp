#include "Window.h"

Window::Window(const std::basic_string<TCHAR> &className,
	const std::basic_string<TCHAR> &windowName,
	int nX, int nY, int nWidth, int nHeight,
	HINSTANCE hIn, void* pData) {

	m_hWnd = CreateWindow(className.c_str(), windowName.c_str(),
		WS_OVERLAPPEDWINDOW, nX, nY, nWidth, nHeight, nullptr,
		nullptr, hIn, pData);
}

Window::Window(const std::basic_string<TCHAR> &className,
	const std::basic_string<TCHAR> &windowName,
	HINSTANCE hIn, void* pData) {

	m_hWnd = CreateWindow(className.c_str(), windowName.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hIn, pData);
}

Window::~Window() {
	if (IsWindow(m_hWnd))
		DestroyWindow(m_hWnd);
}

void Window::Show(bool wantShow) {
	ShowWindow(m_hWnd, wantShow ? SW_SHOW : SW_HIDE);
}

HWND Window::GetHwnd() const {
	return m_hWnd;
}
