#include "Renderer.h"

void Render(HDC hDC, RECT winRect) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };

	HBRUSH hMyBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH hOldBrush = SelectObject(hDC, hMyBrush);
	Rectangle(hDC, winRect.left - 1, winRect.top - 1, winRect.right, winRect.bottom);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hMyBrush);

	for (int y = 0; y < mapSize.cy; y++) {
		for (int x = 0; x < mapSize.cx; x++) {
			int xPos = x * sizeOfCell.cx;
			int yPos = y * sizeOfCell.cy;

			switch (cellMap[y][x]) {
			case '0': break;

			case '1':
				Rectangle(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);
				break;

			case '2':
				Ellipse(hDC, xPos, yPos, xPos + sizeOfCell.cx, yPos + sizeOfCell.cy);
				break;

			case '\n':
				y++;
				x = -1;
				break;
			}
		}
	}
}