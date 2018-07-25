#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wndClass;

	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpszClassName = TEXT("Note Pad");
	wndClass.lpszMenuName = TEXT("blAs1N's Note Pad");
	wndClass.lpfnWndProc = WndProc;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = hIn;
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
	static TCHAR* str;
	static int count;
	static int posX[10];
	static int nowY;
	static RECT rect;

	switch (iMsg) {
	case WM_CREATE:
		str = (TCHAR*)malloc(1000 * sizeof(TCHAR));
		memset(posX, 0, sizeof(posX));
		memset(str, 0, 1000 * sizeof(TCHAR));
		GetClientRect(hWnd, &rect);
		count = 0;
		nowY = 0;
		break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
		DrawText(hDC, str, _tcslen(str), &rect, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;

	case WM_CHAR:
		if (wParam == VK_BACK) {
			posX[nowY]--;

			if (str[--count] == '\n') {
				posX[--nowY]--;
				count--;
			}

			str[count] = 0;
		}

		if (nowY < 10) {
			if (wParam == VK_RETURN || posX[nowY] > 99) {
				if (++nowY > 9) break;
				posX[nowY] = 0;
				str[count++] = '\n';
			}
			else if (wParam != VK_BACK) {
				str[count++] = wParam;
				posX[nowY]++;
			}

			str[count] = 0;
		}

		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}

	case WM_DESTROY:
		str = NULL;
		free(str);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}