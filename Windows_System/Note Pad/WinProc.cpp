#include "WinProc.h"
#include "Window.h"

LRESULT CALLBACK WinProc::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	static std::basic_string<TCHAR> writeList;
	static UINT fontSize = 30;

	switch (iMsg) {
	case WM_CREATE: {
		CreateCaret(hWnd, NULL, 1, 25);
		SetCaretPos(2, 4);
		ShowCaret(hWnd);

		break;
	}
	case WM_CHAR: {
		if (wParam == VK_BACK) {
			if (!(writeList.empty()))
				writeList.erase(writeList.end() - 1, writeList.end());
		}

		else if (wParam == VK_TAB)
			writeList += TEXT("    ");

		else
			writeList += static_cast<TCHAR>(wParam);

		InvalidateRect(hWnd, NULL, true);
		break;
	}

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);

		HFONT hMyFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, TEXT("consolas"));
		HFONT hOldFont = reinterpret_cast<HFONT>(SelectObject(hDC, hMyFont));

		RECT area;
		GetClientRect(hWnd, &area);

		DrawText(hDC, writeList.c_str(), writeList.length(), &area, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);

		int line = 0;
		for (auto i : writeList)
			if (i == VK_RETURN) { line++; }

		int width, lastNewLine;
		GetCharWidth(hDC, NULL, NULL, &width);

		lastNewLine = writeList.rfind(VK_RETURN);
		if (lastNewLine == std::string::npos) { lastNewLine = 0; }
		else { lastNewLine++; }

		SetCaretPos((writeList.length() - lastNewLine) * width, 4 + (line * fontSize));

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, true);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

WinProc::WinProc(HINSTANCE hIn) {
	this->style = CS_OWNDC;
	this->lpfnWndProc = WndProc;
	this->cbClsExtra = 0;
	this->cbWndExtra = 0;
	this->hInstance = hIn;
	this->hIcon = LoadIcon(NULL, IDC_ICON);
	this->hCursor = LoadCursor(NULL, IDC_ARROW);
	this->hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	this->lpszMenuName = NULL;
	this->lpszClassName = TEXT("NotePad");

	RegisterClass(this);
}

WinProc::~WinProc() {}

WinProc::str WinProc::GetName() const {
	return this->lpszClassName;
}