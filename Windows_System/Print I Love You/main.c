#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = TEXT("I love you");
	wndClass.lpszMenuName = TEXT("No");
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hIn;

	RegisterClassEx(&wndClass);

	hWnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszMenuName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hIn, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
	case WM_CREATE:
		srand((unsigned int)time(NULL));
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
		RECT rect;
		RECT winSize;
		GetClientRect(hWnd, &winSize);

		rect.top = rand() % winSize.bottom + winSize.top;
		rect.bottom = rect.top + 100;
		rect.left = rand() % winSize.right + winSize.left;
		rect.right = rect.left + 300;

		TextOut(hDC, rand() % winSize.right + winSize.left, rand() % winSize.bottom + winSize.top, TEXT("I love you"), _tcslen(TEXT("I love you")));
		DrawText(hDC, TEXT("I love you"), _tcslen(TEXT("I love you")), &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}