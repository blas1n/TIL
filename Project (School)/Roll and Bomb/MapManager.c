#include "MapManager.h"

void MapRead() {
	char mapName[11];
	sprintf(mapName, "Stage%d.txt", nowStage + 1);

	FILE* mapFile = fopen(mapName, "r");

	mapSize = GetSize(mapFile);

	cellMap = (char**)malloc(mapSize.cy * sizeof(char*));

#pragma omp parallel for
	for (int i = 0; i < mapSize.cy; i++) {
		cellMap[i] = (char*)malloc(mapSize.cx + 1);
		memset(cellMap[i], 0, mapSize.cx + 1);
		fscanf(mapFile, "%s", cellMap[i]);

		// 플레이어가 해당 위치에 있는지 확인
#pragma omp parallel for
		for (int j = 0; j < mapSize.cx; j++) {
			if (cellMap[i][j] == '2') {
				playerPos.x = j;
				playerPos.y = i;
			}
		}
	}

	fclose(mapFile);
}

SIZE GetSize(FILE* mapFile) {
	SIZE mapSize;
	int size;

	// 파일의 끝으로가 전체 크기를 구한다.
	fseek(mapFile, 0, SEEK_END);
	size = ftell(mapFile);

	char tempMap[50];

	// 한줄 끝으로가 Y값을 구하고 전체와 나눠 X값을 구한다.
	rewind(mapFile);
	fgets(tempMap, 50, mapFile);
	mapSize.cx = ftell(mapFile) - 1;
	mapSize.cy = size / mapSize.cx;
	rewind(mapFile);

	return mapSize;
}

void MapDataFree() {
	for (int i = 0; i < mapSize.cy; i++) {
		cellMap[i] = NULL;
		free(cellMap[i]);
	}

	cellMap = NULL;
	free(cellMap);
}

void PlayerMoveInCell(POINT newPos) {
	cellMap[playerPos.y][playerPos.x] = '0';
	cellMap[newPos.y][newPos.x] = '2';

	// 이전 위치를 beforePos에 저장하고
	// 새로운 위치를 playerPos에 저장한다.
	beforePos = playerPos;
	playerPos = newPos;
}