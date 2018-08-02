#include "EnemyManager.h"

void WINAPI AI(LPVOID addressOfPos) {
	int wayPos[2] = { -1, 1 };
	BOOL isAlive = TRUE;

	POINT myPos = *((POINT*)addressOfPos);
	SAFE_FREE(addressOfPos);

	POINT movePos[5];
	POINT goodPos[5];
	POINT subGoodPos[5];

	int numOfGoodPos, numOfSubGoodPos, choice;

	while (isAlive && !bEnd) {
		if (mapLoad) continue;

		isAlive = EnemyCheckAlive(myPos);
		EnemyAttack(myPos);
		Sleep(250);

		numOfGoodPos = 0;
		numOfSubGoodPos = 0;

		// Point형 변수를 정리한다.
#pragma omp parallel for
		for (int i = 0; i < 5; i++) {
			goodPos[i].x = 0;
			goodPos[i].y = 0;

			subGoodPos[i].x = 0;
			subGoodPos[i].y = 0;

			movePos[i] = myPos;
		}

		movePos[0].y -= 1;
		movePos[1].x -= 1;
		movePos[3].x += 1;
		movePos[4].y += 1;

		// 장애물이 없는 위치와 최선의 위치를 고른다.
#pragma omp parallel for
		for (int i = 0; i < 5; i++) {
			if (cellMap[movePos[i].y][movePos[i].x] != '0') continue;
			if (bombMap[movePos[i].y][movePos[i].x] == '1') continue;
			subGoodPos[numOfSubGoodPos++] = movePos[i];

			if (bombMap[movePos[i].y][movePos[i].x] == '2') continue;

			if (i == 0) {
				if (bombMap[movePos[i].y - 1][movePos[i].x] != '0') continue;
				if (bombMap[movePos[i].y][movePos[i].x - 1] != '0') continue;
				if (bombMap[movePos[i].y][movePos[i].x + 1] != '0') continue;
			}
				
			else if (i == 1) {
				if (bombMap[movePos[i].y][movePos[i].x - 1] != '0') continue;
				if (bombMap[movePos[i].y - 1][movePos[i].x] != '0') continue;
				if (bombMap[movePos[i].y + 1][movePos[i].x] != '0') continue;
			}

			else if (i == 3) {
				if (bombMap[movePos[i].y + 1][movePos[i].x] != '0') continue;
				if (bombMap[movePos[i].y][movePos[i].x - 1] != '0') continue;
				if (bombMap[movePos[i].y][movePos[i].x + 1] != '0') continue;
			}

			else if (i == 4) {
				if (bombMap[movePos[i].y][movePos[i].x + 1] != '0') continue;
				if (bombMap[movePos[i].y - 1][movePos[i].x] != '0') continue;
				if (bombMap[movePos[i].y + 1][movePos[i].x] != '0') continue;
			}

			goodPos[numOfGoodPos++] = movePos[i];
		}
		
		// 이동할 좋은 위치가 있을 경우
		if (numOfGoodPos > 0) {
			choice = rand() % numOfGoodPos;

			cellMap[myPos.y][myPos.x] = '0';
			cellMap[goodPos[choice].y][goodPos[choice].x] = '3';

			myPos = goodPos[choice];
		}

		// 이동할 좋은 위치가 없을 경우
		else {
			// 진퇴 양난의 경우
			if (!numOfSubGoodPos) continue;

			choice = rand() % numOfSubGoodPos;

			cellMap[myPos.y][myPos.x] = '0';
			cellMap[subGoodPos[choice].y][subGoodPos[choice].x] = '3';

			myPos = subGoodPos[choice];
		}
	}

	cellMap[myPos.y][myPos.x] = '0';
	nowKill++;
}

void EnemyAttack(POINT myPos) {
	for (int y = myPos.y - 1; y <= myPos.y + 1; y++) {
		for (int x = myPos.x - 1; x <= myPos.x + 1; x++) {
			if (cellMap[y][x] == '2')
				InstallBomb(myPos);
		}
	}
}

BOOL EnemyCheckAlive(POINT pos) {
	if (bombMap[pos.y][pos.x] == '2')
		return FALSE;

	return TRUE;
}