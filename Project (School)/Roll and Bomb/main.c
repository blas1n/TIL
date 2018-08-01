#include "main.h"

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	m_hInstance = hIn;

	HWND hWnd;
	MSG msg;
	WNDCLASSEX wndClass;
	BOOL bDone = FALSE;

	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = TEXT("Roll and Bomb");
	wndClass.lpszMenuName = TEXT("Roll and Bomb");
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hIn;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	
	RegisterClassEx(&wndClass);

	hWnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszMenuName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hIn, NULL);

	Init();

	ShowWindow(hWnd, cShow);
	UpdateWindow(hWnd);

	while (!bDone) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else InvalidateRgn(hWnd, NULL, FALSE);

		if (!Update()) {
			cellMap[playerPos.y][playerPos.x] = '0';
		}
	}

	Release();
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC;

	static HDC hBackDC;
	static HBITMAP hMyBitmap, hOldBitmap;
	static HBRUSH hMyBrush, hOldBrush;
	
	static DWORD attackCool;

	switch (iMsg) {
	case WM_CREATE:
		attackCool = timeGetTime();
		GetClientRect(hWnd, &winRect);

		hDC = GetDC(hWnd);
		hBackDC = CreateCompatibleDC(hDC);

		hMyBitmap = CreateCompatibleBitmap(hDC, winRect.right, winRect.bottom);
		hOldBitmap = (HBITMAP)SelectObject(hBackDC, hMyBitmap);

		hMyBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hOldBrush = (HBRUSH)SelectObject(hBackDC, hMyBrush);

		ReleaseDC(hWnd, hDC);
		break;

	case WM_DESTROY:
		SelectObject(hBackDC, hOldBitmap);
		DeleteObject(hMyBitmap);

		SelectObject(hBackDC, hOldBrush);
		DeleteObject(hMyBrush);

		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		Move(wParam);
		break;

	case WM_LBUTTONDOWN:
		if (1000 < timeGetTime() - attackCool) {
			attackCool = timeGetTime();
			Attack();
		}
		break;

	case WM_PAINT: {
		GetClientRect(hWnd, &winRect);
		hDC = BeginPaint(hWnd, &ps);
		
		Render(hBackDC);
		BitBlt(hDC, 0, 0, winRect.right, winRect.bottom, hBackDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void Init() {
	MapRead();
	srand((UINT)time(NULL));
	isAlive = TRUE;
}

void Release() {
	Sleep(1800);
	MapDataFree();
}

BOOL Update() {
	return CheckAlive();
}