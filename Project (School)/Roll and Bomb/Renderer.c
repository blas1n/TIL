#include "Renderer.h"

void Render(HDC hDC) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	Rectangle(hDC, winRect.left - 1, winRect.top - 1, winRect.right, winRect.bottom);

	RenderCell(hDC);
	RenderBomb(hDC);
}

void RenderCell(HDC hDC) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	HBRUSH hBMyBrush = CreateSolidBrush(RGB(100, 100, 100));
	HBRUSH hBOldBrush = (HBRUSH)SelectObject(hDC, hBMyBrush);

	HBRUSH hSMyBrush = CreateSolidBrush(RGB(0, 200, 250));
	HBRUSH hSOldBrush = (HBRUSH)SelectObject(hDC, hSMyBrush);
	SelectObject(hDC, hSOldBrush);

	for (int y = 0; y < mapSize.cy; y++) {
		for (int x = 0; x < mapSize.cx; x++) {
			int xPos = x * sizeOfCell.cx;
			int yPos = y * sizeOfCell.cy;

			if (cellMap[y][x] == '1')
				Rectangle(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);

			else if (cellMap[y][x] == '2') {
				hSOldBrush = (HBRUSH)SelectObject(hDC, hSMyBrush);
				Ellipse(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);
				SelectObject(hDC, hSOldBrush);
			}
		}
	}

	SelectObject(hDC, hBOldBrush);
	DeleteObject(hBMyBrush);
	DeleteObject(hSMyBrush);
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