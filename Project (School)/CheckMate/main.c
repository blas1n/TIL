#include "main.h"

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	m_hInstance = hIn;

	HWND hWnd;
	MSG msg;
	WNDCLASSEX wndClass;
	BOOL bDone = FALSE;

	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = TEXT("CheckMate!");
	wndClass.lpszMenuName = TEXT("CheckMate!");
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hIn;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hIn, IDI_CHESE);
	wndClass.hIconSm = wndClass.hIcon;

	RegisterClassEx(&wndClass);

	hWnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszMenuName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hIn, NULL);

	Init();

	ShowCursor(FALSE);
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
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

		if (cellMap == NULL) continue;

		if (!Update())
			cellMap[playerPos.y][playerPos.x] = '0';
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

	m_hWnd = hWnd;

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

		SetTimer(hWnd, 0, 1000, (TIMERPROC)IncreaseTime);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);

		SelectObject(hBackDC, hOldBitmap);
		DeleteObject(hMyBitmap);

		SelectObject(hBackDC, hOldBrush);
		DeleteObject(hMyBrush);

		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (cellMap == NULL)
			NextMap();

		else if ((!isAlive || bClear) && (wParam == VK_RETURN || wParam == VK_ESCAPE))
			SendMessage(hWnd, WM_CLOSE, 0, 0);

		else
			Move(wParam);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &winRect);

		hDC = GetDC(hWnd);
		hBackDC = CreateCompatibleDC(hDC);

		hMyBitmap = CreateCompatibleBitmap(hDC, winRect.right, winRect.bottom);
		hOldBitmap = (HBITMAP)SelectObject(hBackDC, hMyBitmap);

		ReleaseDC(hWnd, hDC);

		break;

	case WM_PAINT: {
		GetClientRect(hWnd, &winRect);
		hDC = BeginPaint(hWnd, &ps);
		
		if (bClear)
			RenderClear(hBackDC);

		else if (cellMap == NULL)
			RenderStart(hBackDC);

		else if (!isAlive)
			RenderDeath(hBackDC);

		else Render(hBackDC);

		BitBlt(hDC, 0, 0, winRect.right, winRect.bottom, hBackDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void Init() {
	srand((UINT)time(NULL));
	bEnd = FALSE;
	bClear = FALSE;
	isAlive = TRUE;
	nowStage = STARTSTAGE;
}

void Release() {
	bEnd = TRUE;

	Sleep(2000);
	MapDataFree();
}

BOOL Update() {
	switch (nowStage) {
	case 1:
		if (5 <= nowKill) {
			nowKill = 0;
			NextMap();
		}
			
		break;
	case 2:
		if (12 <= nowKill) {
			nowKill = 0;
			NextMap();
		}
		break;
	case 3:
		if (15 <= nowKill) {
			nowKill = 0;
			NextMap();
		}
		break;
	case 4:
		if (15 <= nowKill) {
			nowKill = 0;
			NextMap();
		}
		break;
	case 5:
		if (20 <= nowKill) {
			nowKill = 0;
			NextMap();
		}
		break;
	}

	return CheckAlive();
}