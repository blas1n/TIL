#include <Windows.h>
#include <tchar.h>

#define MAX_LINE (10)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
inline void SafeFree(void*);
int GetMyFontWidth(HWND, UINT);
RECT* GetRects(HWND, UINT);

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
	static TCHAR* writeStr[MAX_LINE];
	static RECT* windowRect;
	static int counts[MAX_LINE];
	static int nowWidth = 0;
	static const UINT fontSize = 23;

	switch (iMsg) {
	case WM_CREATE:
#pragma omp parallel for
		for (int i = 0; i < MAX_LINE; i++) {
			writeStr[i] = (TCHAR*)malloc(100 * sizeof(TCHAR));
			memset(writeStr[i], (TCHAR)0, 100 * sizeof(TCHAR));
		}

		windowRect = GetRects(hWnd, fontSize);
		memset(counts, 0, sizeof(counts));

		CreateCaret(hWnd, NULL, 2, 17);
		ShowCaret(hWnd);
		break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);

		HFONT hMyFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("consolas"));
		HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

		for (int i = 0; i <= nowWidth; i++)
			DrawText(hDC, writeStr[i], counts[i], &windowRect[i], DT_LEFT | DT_SINGLELINE);
		
		SIZE size;
		if (writeStr[nowWidth][0] != (TCHAR)0)
			GetTextExtentPoint(hDC, writeStr[nowWidth], _tcslen(writeStr[nowWidth]), &size) - 2;
		else {
			size.cx = 0;
			size.cy = GetMyFontWidth(hWnd, fontSize) + 11;
		}

		SetCaretPos(size.cx, nowWidth * size.cy);

		SelectObject(hDC, hOldFont);
		DeleteObject(hMyFont);
		
		EndPaint(hWnd, &ps);
		break;
	}
				   
	case WM_CHAR: {
		if (wParam == VK_BACK) {
			if (nowWidth > 9) nowWidth = 9;
			if (counts[0] > 0) counts[nowWidth]--;

			if (nowWidth > 0 && counts[nowWidth] < 0) {
				counts[nowWidth--] = 0;
				counts[nowWidth]--;
			}
		}

		else if (wParam == VK_RETURN || counts[nowWidth] > 99) {
			if (nowWidth < MAX_LINE - 1) nowWidth++;
		}

		else if (nowWidth < MAX_LINE)
			writeStr[nowWidth][counts[nowWidth]++] = (TCHAR)wParam;

		if (nowWidth < MAX_LINE || wParam == VK_BACK)
			writeStr[nowWidth][counts[nowWidth]] = (TCHAR)0;

		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}

	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();

		for (int i = 0; i < MAX_LINE; i++)
			SafeFree(writeStr[i]);
		SafeFree(windowRect);

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

inline void SafeFree(void* trash) {
	trash = NULL;
	free(trash);
}

int GetMyFontWidth(HWND hWnd, UINT fontSize) {
	int result;

	HDC hDC = GetDC(hWnd);

	HFONT hMyFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("consolas"));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	GetCharWidth(hDC, 0, 0, &result);

	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);

	ReleaseDC(hWnd, hDC);

	return result;
}

RECT* GetRects(HWND hWnd, UINT fontSize) {
	RECT* result = (RECT*)malloc(MAX_LINE * sizeof(RECT));
	int fontWidth = GetMyFontWidth(hWnd, fontSize);

	GetClientRect(hWnd, &result[0]);
	result[0].bottom = fontWidth * 2;

#pragma omp parallel for
	for (int i = 1; i < MAX_LINE; i++) {
		result[i] = result[i - 1];
		result[i].top += fontWidth * 2;
		result[i].bottom = result[i].top + fontWidth * 2;
	}

	return result;
}