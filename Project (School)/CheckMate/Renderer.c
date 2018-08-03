#include "Renderer.h"

void Render(HDC hDC) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	Rectangle(hDC, winRect.left - 1, winRect.top - 1, winRect.right, winRect.bottom);

	RenderCell(hDC);
	RenderBomb(hDC);
	RenderTimer(hDC);
}

void RenderCell(HDC hDC) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	FillRect(hDC, &winRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	for (int y = 0; y < mapSize.cy; y++) {
		for (int x = 0; x < mapSize.cx; x++) {
			int xPos = x * sizeOfCell.cx;
			int yPos = y * sizeOfCell.cy;

			switch (cellMap[y][x]) {
			case '1': {
				HBRUSH hMyBrush = CreateSolidBrush(RGB(100, 100, 100));
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hMyBrush);

				Rectangle(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);

				SelectObject(hDC, hOldBrush);
				DeleteObject(hMyBrush);
				break;
			}

			case '2': {
				HBRUSH hMyBrush = CreateSolidBrush(RGB(0, 200, 250));
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hMyBrush);

				Ellipse(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);

				SelectObject(hDC, hOldBrush);
				DeleteObject(hMyBrush);
				break;
			}

			case '3': {
				HBRUSH hMyBrush = CreateSolidBrush(RGB(255, 255, 0));
				HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hMyBrush);

				Ellipse(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);

				SelectObject(hDC, hOldBrush);
				DeleteObject(hMyBrush);
				break;
			}	
			}
		}
	}
}

void RenderBomb(HDC hDC) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	HBRUSH hMyBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hMyBrush);

	for (int y = 0; y < mapSize.cy; y++) {
		for (int x = 0; x < mapSize.cx; x++) {
			int xPos = x * sizeOfCell.cx;
			int yPos = y * sizeOfCell.cy;

			if (bombMap[y][x] == '1')
				Ellipse(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);
					
			else if (bombMap[y][x] == '2')
				Rectangle(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);
		}
	}

	SelectObject(hDC, hOldBrush);
	DeleteObject(hMyBrush);
}

void RenderDeath(HDC hDC) {
	FillRect(hDC, &winRect, CreateSolidBrush(RGB(75, 0, 150)));

	int sizeOfFont = winRect.right / 10;

	HFONT hMyFont = CreateFont(sizeOfFont, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		0, 0, 0, 0, TEXT("Consolas"));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, RGB(75, 0, 150));

	RECT captionRect = winRect;
	captionRect.bottom = winRect.bottom / 4 * 3;
	captionRect.top = winRect.bottom / 3;

	DrawText(hDC, TEXT("적이 자폭해야지\n왜 너가 자폭해"), (int)_tcslen(TEXT("적이 자폭해야지\n왜 너가 자폭해")), &captionRect, DT_CENTER);

	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);
}

void RenderLoad(HDC hDC) {
	FillRect(hDC, &winRect, CreateSolidBrush(RGB(80, 130, 50)));

	int sizeOfFont = winRect.right / 8;

	HFONT hMyFont = CreateFont(sizeOfFont, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		0, 0, 0, 0, TEXT("Consolas"));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkColor(hDC, RGB(80, 130, 50));

	DrawText(hDC, TEXT("맵 로딩 중..."), (int)_tcslen(TEXT("맵 로딩 중...")), &winRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);
}

void RenderStart(HDC hDC) {
	FillRect(hDC, &winRect, CreateSolidBrush(RGB(200, 255, 200)));

	int sizeOfFont = winRect.right / 6;

	HFONT hMyFont = CreateFont(sizeOfFont, 0, 0, 0, FW_BOLD, 0, 0, 0,
		ANSI_CHARSET, 0, 0, 0, 0, TEXT("Consolas"));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	SetTextColor(hDC, RGB(50, 0, 50));
	SetBkColor(hDC, RGB(200, 255, 200));

	DrawText(hDC, TEXT("CheckMate!"), (int)_tcslen(TEXT("CheckMate!")), &winRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);
}

void RenderTimer(HDC hDC) {
	RECT timerRect;
	timerRect.bottom = winRect.bottom;
	timerRect.right = winRect.right;
	timerRect.top = timerRect.bottom - 50;
	timerRect.left = timerRect.right - 100;

	Time takeTime = GetTakeTime();
	TCHAR timeToString[8];

#ifdef UNICODE
	swprintf(timeToString, 8, L"%02d : %02d", takeTime.minute, takeTime.second);
#elif
	sprintf(timeToString, 8, "%02d : %02d", takeTime.minute, takeTime.second);
#endif

	int sizeOfFont = (timerRect.right - timerRect.left) / 4;

	HFONT hMyFont = CreateFont(sizeOfFont, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		0, 0, 0, 0, TEXT("Consolas"));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	SetTextColor(hDC, RGB(50, 0, 50));
	SetBkColor(hDC, RGB(200, 255, 200));

	DrawText(hDC, timeToString, (int)_tcslen(timeToString), &timerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);
}

void RenderClear(HDC hDC) {
	Time clearTime = GetTakeTime();
	TCHAR timeToString[30];

#ifdef UNICODE
	swprintf(timeToString, 30, L"클리어에 걸린 시간 : %02d : %02d", clearTime.minute, clearTime.second);
#elif
	sprintf(timeToString, 30, "클리어에 걸린 시간 : %02d : %02d", clearTime.minute, clearTime.second);
#endif
	
	RECT subCaptionRect = winRect;
	subCaptionRect.bottom = subCaptionRect.bottom / 4 * 3;
	subCaptionRect.top = subCaptionRect.bottom / 2;

	int sizeOfCaption = winRect.bottom / 5;
	int sizeOfSubCaption = (subCaptionRect.bottom - subCaptionRect.top) / 9;

	HFONT hMyFont = CreateFont(sizeOfCaption, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET,
		0, 0, 0, 0, TEXT("Consolas"));
	HFONT hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	SetBkColor(hDC, RGB(180, 40, 40));

	FillRect(hDC, &winRect, CreateSolidBrush(RGB(180, 40, 40)));
	DrawText(hDC, TEXT("클리어!!"), (int)_tcslen(TEXT("클리어!!")), &winRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);

	hMyFont = CreateFont(sizeOfSubCaption, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		0, 0, 0, 0, TEXT("Consolas"));
	hOldFont = (HFONT)SelectObject(hDC, hMyFont);

	SetBkColor(hDC, RGB(180, 40, 40));

	DrawText(hDC, timeToString, 30, &subCaptionRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	SelectObject(hDC, hOldFont);
	DeleteObject(hMyFont);
}