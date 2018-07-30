#include "Renderer.h"

void Init() {
	nowStage = 0;
	MapRead();
}

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

void Release() {
	for (int i = 0; i < mapSize.cy; i++) {
		cellMap[i] = NULL;
		free(cellMap[i]);
	}
		
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
	fseek(mapFile, 0, SEEK_SET);

	cellMap = (char**)malloc(mapSize.cy * sizeof(char*));

	for (int i = 0; i < mapSize.cy; i++) {
		cellMap[i] = (char*)malloc(mapSize.cx + 1);
		memset(cellMap[i], 0, mapSize.cx + 1);
		fscanf(mapFile, "%s", cellMap[i]);
	}

	fclose(mapFile);
}