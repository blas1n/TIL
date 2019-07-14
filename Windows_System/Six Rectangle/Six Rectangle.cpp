#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
	case WM_LBUTTONDOWN: {
		const unsigned int Width = 100;
		const unsigned int Height = 100;

		HDC hDC = nullptr;
		HBRUSH hMyBrush = nullptr;
		HBRUSH hOldBrush = nullptr;
		RECT rcOutput = {};

		// DC를 얻어서 hDC에 저장
		hDC = GetDC(hWnd);

		// HS를 다 돌기 위해서 총 6번 실행
		for (unsigned int i = 0; i < 6; i++) {
			// 브러쉬를 생성하고, 이전 브러쉬를 저장한다
			hMyBrush = CreateHatchBrush(i, RGB(150, 255, 0));
			hOldBrush = reinterpret_cast<HBRUSH>(SelectObject(hDC, hMyBrush));

			// 사각형의 좌, 상, 우, 하 의 위치를 지정한다
			rcOutput.left = 10 + ((i % 3) * (Width + 10));
			rcOutput.top = 10 + ((i / 3) * (Height + 10));
			rcOutput.right = rcOutput.left + Width;
			rcOutput.bottom = rcOutput.top + Height;

			// rcOutput을 이용해 사각형을 그린다
			Rectangle(hDC, rcOutput.left, rcOutput.top, rcOutput.right, rcOutput.bottom);

			// 브러쉬를 이전 브러쉬로 바꾼 후, 브러쉬를 삭제한다.
			SelectObject(hDC, hOldBrush);
			DeleteObject(hMyBrush);
		}

		// DC를 반환하고, 종료
		ReleaseDC(hWnd, hDC);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hin, HINSTANCE hPrev, PSTR cmd, int Cshow) {
	HWND hWnd;
	WNDCLASS wnd;
	MSG msg;

	// 구조체 wnd의 멤버를 초기화
	wnd.style = CS_OWNDC;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hin;
	wnd.hIcon = LoadIcon(NULL, IDC_ICON);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = TEXT("MyWindow");

	RegisterClass(&wnd);

	// 윈도우를 생성하여 HWND인 hWnd에 저장
	hWnd = CreateWindow(TEXT("MyWindow"), TEXT("Sample App"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hin, NULL);

	// 방금 생성한 윈도우를 보여줌
	ShowWindow(hWnd, SW_SHOW);

	// 창이 바로 종료 되지 않도록 While로 반복
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}