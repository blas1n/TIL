#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(HINSTANCE hin, HINSTANCE hPrev, LPTSTR cmd, int Cshow) {
	HWND hWnd;
	WNDCLASS wnd;
	MSG msg;

	// wnd의 멤버 초기화
	wnd.style = CS_OWNDC;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hin;
	wnd.hIcon = LoadIcon(NULL, IDC_ICON);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH));
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = TEXT("MyWindow");

	RegisterClass(&wnd);

	// 윈도우 생성 -> hWnd에 저장
	hWnd = CreateWindow(TEXT("MyWindow"), TEXT("Six Rectangle (Pen.Ver)"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hin, NULL);

	// 윈도우를 보이게 함
	ShowWindow(hWnd, SW_SHOW);

	// 창이 바로 종료 되지 않도록 While로 반복
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
	case WM_LBUTTONDOWN: {
		// 높이와 길이를 상수로 정의
		const unsigned int Width = 100;
		const unsigned int Height = 100;

		// DC, HPEN 2개, HBRUSH 2개, HFON 2개, 사각형 정의
		HDC hDC = nullptr;
		HPEN hMyPen = nullptr;
		HPEN hOldPen = nullptr;
		HBRUSH hMyBrush = nullptr;
		HBRUSH hOldBrush = nullptr;
		HFONT hMyFont = nullptr;
		HFONT hOldFont = nullptr;
		RECT rcOutput = {};

		// DC를 얻어서 hDC에 저장
		hDC = GetDC(hWnd);

		for (unsigned int i = 0; i < 6; i++) {
			// 펜과 브러시를 차례대로 변경, 이전 상태 저장
			hMyPen = CreatePen(i, 1, RGB(0, 255, 0));
			hOldPen = reinterpret_cast<HPEN>(SelectObject(hDC, hMyPen));

			hMyBrush = CreateSolidBrush(RGB(30, 170, 255));
			hOldBrush = reinterpret_cast<HBRUSH>(SelectObject(hDC, hMyBrush));

			hMyFont = CreateFont(100, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("Hack"));
			hOldFont = reinterpret_cast<HFONT>(SelectObject(hDC, hMyFont));

			// 사각형의 좌, 상, 우, 하 의 위치를 지정한다
			rcOutput.left = 10 + ((i % 3) * (Width + 10));
			rcOutput.top = 120 + ((i / 3) * (Height + 10));
			rcOutput.right = rcOutput.left + Width;
			rcOutput.bottom = rcOutput.top + Height;

			// 사각형을 그림
			Rectangle(hDC, rcOutput.left, rcOutput.top, rcOutput.right, rcOutput.bottom);
			TextOut(hDC, 10, 10, TEXT("blAs1N"), 6);

			// GDI 정리 (이전으로 복구 후 삭제)
			SelectObject(hDC, hOldPen);
			DeleteObject(hMyPen);

			SelectObject(hDC, hOldBrush);
			DeleteObject(hMyBrush);

			SelectObject(hDC, hOldFont);
			DeleteObject(hMyFont);
		}

		// hDC를 반환
		ReleaseDC(hWnd, hDC);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}