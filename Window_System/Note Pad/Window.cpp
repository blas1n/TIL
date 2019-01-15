#include "Window.h"

Window::Window(const str& className, const str& windowName,
	int nX, int nY, int nWidth, int nHeight,
	HINSTANCE hIn, void* pData) {

	m_hWnd = CreateWindow(className.c_str(), windowName.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | ES_AUTOHSCROLL |
		ES_MULTILINE, nX, nY, nWidth, nHeight, nullptr,
		nullptr, hIn, pData);
}

Window::Window(const str& className, const str& windowName,
	HINSTANCE hIn, void* pData) {

	m_hWnd = CreateWindow(className.c_str(), windowName.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | ES_AUTOHSCROLL |
		ES_MULTILINE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, nullptr, nullptr, hIn, pData);
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