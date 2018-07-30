#include "Renderer.h"

void Init() {
	nowStage = 0;
	MapRead();
}

void Render(HDC hDC, RECT winRect) {
	const SIZE sizeOfCell = { winRect.right / (mapSize.cx - 1), winRect.bottom / mapSize.cy };
	int x = 0, y = 0;

	HBRUSH hMyBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH hOldBrush = SelectObject(hDC, hMyBrush);
	Rectangle(hDC, winRect.left - 1, winRect.top - 1, winRect.right, winRect.bottom);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hMyBrush);

	for (int i = 0; i < mapSize.cx * mapSize.cy; i++) {
		int xPos = x * sizeOfCell.cx;
		int yPos = y * sizeOfCell.cy;

		switch (cellMap[i]) {
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

		x++;
	}
}

void Release() {
	cellMap = NULL;
	free(cellMap);
}

void MapRead() {
	char mapName[11];
	char tempMap[50];
	sprintf(mapName, "Stage%d.txt", nowStage + 1);

	FILE* mapFile = fopen(mapName, "r");

	fseek(mapFile, 0, SEEK_END);
	int size = ftell(mapFile);

	fseek(mapFile, 0, SEEK_SET);
	fgets(tempMap, 50, mapFile);
	mapSize.cx = ftell(mapFile) - 1;
	mapSize.cy = size / mapSize.cx;

	cellMap = (char*)malloc((mapSize.cx * mapSize.cy) + 1);

	memset(cellMap, 0, (mapSize.cx * mapSize.cy) + 1);
	fseek(mapFile, 0, SEEK_SET);

	fread(cellMap, mapSize.cx * mapSize.cy, 1, mapFile);

	fclose(mapFile);
}