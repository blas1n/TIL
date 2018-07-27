#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wndClass;

	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = TEXT("Draw Ellipse");
	wndClass.lpszMenuName = TEXT("Ellipse");
	wndClass.hInstance = hIn;
	wndClass.lpfnWndProc = WndProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	
	RegisterClassEx(&wndClass);

	hWnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszMenuName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hIn, NULL);

	ShowWindow(hWnd, cShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);

		HBRUSH hMyBrush = CreateSolidBrush(RGB(255, 192, 203));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hMyBrush);

		HPEN hMyPen = CreatePen(PS_DASH, 1, RGB(255, 255, 0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hMyPen);

		Ellipse(hDC, 160, 80, 240, 160);

		SelectObject(hDC, hOldPen);
		DeleteObject(hMyPen);
		
		SelectObject(hDC, hOldBrush);
		DeleteObject(hMyBrush);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}