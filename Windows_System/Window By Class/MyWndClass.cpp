#include "MyWndClass.h"

MyWndClass::MyWndClass(HINSTANCE hIn) {
	this->style = CS_OWNDC;
	this->lpfnWndProc = WndProc;
	this->cbClsExtra = 0;
	this->cbWndExtra = 0;
	this->hInstance = hIn;
	this->hIcon = LoadIcon(NULL, IDC_ICON);
	this->hCursor = LoadCursor(NULL, IDC_ARROW);
	this->hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	this->lpszMenuName = NULL;
	this->lpszClassName = TEXT("MyWindow");

	RegisterClass(this);
}

MyWndClass::~MyWndClass() {}

LRESULT CALLBACK MyWndClass::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

std::basic_string<TCHAR> MyWndClass::GetName() const {
	return this->lpszClassName;
}