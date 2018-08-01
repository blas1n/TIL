#include "Renderer.h"

void Render(HDC hDC) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	Rectangle(hDC, winRect.left - 1, winRect.top - 1, winRect.right, winRect.bottom);

	RenderCell(hDC);
	RenderBomb(hDC);
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