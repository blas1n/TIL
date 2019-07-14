#include <Windows.h>
#include <tchar.h>
#include <string>
#include <list>

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
	wnd.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = TEXT("MyWindow");

	RegisterClass(&wnd);

	// 윈도우 생성 -> hWnd에 저장
	hWnd = CreateWindow(TEXT("MyWindow"), TEXT("그림판"),
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
	static bool isMouseDown = false;

	static std::list<POINT> draw;
	static std::list<int> thickness;
	static std::list<COLORREF> color;
	static std::list<bool> click;

	static int nowThickness = 3;
	static COLORREF nowColor = RGB(0, 0, 0);

	switch (iMsg) {
	case WM_LBUTTONDOWN: {
		isMouseDown = true;

		draw.push_back(POINT{ LOWORD(lParam), HIWORD(lParam) });
		thickness.push_back(nowThickness);
		color.push_back(nowColor);
		click.push_back(isMouseDown);

		break;
	}
	case WM_LBUTTONUP: {
		isMouseDown = false;
		draw.push_back(POINT{ LOWORD(lParam), HIWORD(lParam) });
		thickness.push_back(nowThickness);
		color.push_back(nowColor);
		click.push_back(isMouseDown);
		break;
	}
	case WM_MOUSEMOVE: {
		if (isMouseDown) {
			draw.push_back(POINT{ LOWORD(lParam), HIWORD(lParam) });
			thickness.push_back(nowThickness);
			color.push_back(nowColor);
			click.push_back(isMouseDown);

			InvalidateRect(hWnd, NULL, false);
		}
		break;
	}
	case WM_MOUSEWHEEL: {
		int wheelMove = GET_WHEEL_DELTA_WPARAM(wParam);
		nowThickness += wheelMove > 0 ? 1 : -1;

		if (nowThickness > 40) { nowThickness = 40; }
		else if (nowThickness < 1) { nowThickness = 1; }

		InvalidateRect(hWnd, NULL, false);

		break;
	}
	case WM_KEYDOWN: {
		HDC hDC = GetDC(hWnd);
		TCHAR key = wParam;

		// q는 검은 색, w는 붉은 색, e는 파랑 색,  r은 초록색
		switch (key)
		{
		case 'Q':
			nowColor = RGB(0, 0, 0);
			break;
		case 'W':
			nowColor = RGB(255, 0, 0);
			break;
		case 'E':
			nowColor = RGB(0, 0, 255);
			break;
		case 'R':
			nowColor = RGB(0, 255, 0);
			break;
		}

		break;
	}
	case WM_PAINT: {
		bool upEdge = false;
		PAINTSTRUCT ps = {};

		HDC hDC = BeginPaint(hWnd, &ps);
		HPEN hMyPen = nullptr;
		HPEN hOldPen = nullptr;

		if (draw.size() > 1) {
			std::list<int>::iterator thickIter = thickness.begin();
			std::list<COLORREF>::iterator colorIter = color.begin();
			std::list<bool>::iterator clickIter = click.begin();

			MoveToEx(hDC, draw.front().x, draw.front().y, nullptr);

			for (auto i : draw) {
				hMyPen = CreatePen(PS_SOLID, *(thickIter++), *(colorIter++));
				hOldPen = reinterpret_cast<HPEN>(SelectObject(hDC, hMyPen));

				if (!(*(clickIter)))
					upEdge = true;

				else {
					if ((*(clickIter)) && upEdge) {
						upEdge = false;
						MoveToEx(hDC, i.x, i.y, nullptr);
					}

					LineTo(hDC, i.x, i.y);
				}
					
				clickIter++;
				SelectObject(hDC, hOldPen);
				DeleteObject(hMyPen);
			}
		}

		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hDC);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}