#include "Renderer.h"

void Init() {
	FILE* mapFile = fopen("Stage1.txt", "r");

	fseek(mapFile, 0, SEEK_END);
	mapSize = ftell(mapFile);
	cellMap = (char*)malloc(mapSize + 1);
	memset(cellMap, 0, mapSize + 1);
	fseek(mapFile, 0, SEEK_SET);

	fread(cellMap, mapSize, 1, mapFile);

	fclose(mapFile);
}

void Render(HDC hDC, RECT winRect) {
	const int sizeOfCell = 30;
	int x = 0, y = 0;

	HBRUSH hMyBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH hOldBrush = SelectObject(hDC, hMyBrush);
	Rectangle(hDC, winRect.left - 1, winRect.top - 1, winRect.right, winRect.bottom);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hMyBrush);

	for (int i = 0; i < mapSize; i++) {
		switch (cellMap[i]) {
		case '0':
			x++;
			break;
		case '1':
			Rectangle(hDC, x * sizeOfCell, y * sizeOfCell, x * sizeOfCell + sizeOfCell, y * sizeOfCell + sizeOfCell);
			x++;
			break;
		case '\n':
			y++;
			x = 0;
			break;
		}
	}
}

void Release() {
	cellMap = NULL;
	free(cellMap);
}