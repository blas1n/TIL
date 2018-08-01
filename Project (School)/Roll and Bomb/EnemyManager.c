#include "EnemyManager.h"

void WINAPI AI(LPVOID addressOfPos) {
	BOOL isAlive = TRUE;

	POINT myPos = *((POINT*)addressOfPos);
	SAFE_FREE(addressOfPos);

	POINT nowPos = myPos;

	while (isAlive) {
		do {
			int way = rand() % 4;

			switch (way) {
			case FRONT:
				if (cellMap[nowPos.y - 1][nowPos.x] == '0')
					nowPos.y--;
				break;
			case BACK:
				if (cellMap[nowPos.y + 1][nowPos.x] == '0')
					nowPos.y++;
				break;
			case LEFT:
				if (cellMap[nowPos.y][nowPos.x - 1] == '0')
					nowPos.x--;
				break;
			case RIGHT:
				if (cellMap[nowPos.y][nowPos.x + 1] == '0')
					nowPos.x++;
				break;
			}
		} while (nowPos.x == myPos.x && nowPos.y == myPos.y);

		cellMap[myPos.y][myPos.x] = '0';
		cellMap[nowPos.y][nowPos.x] = '3';

		myPos = nowPos;
		isAlive = EnemyCheckAlive(myPos);
		Sleep(300);
	}

	cellMap[myPos.y][myPos.x] = '0';
}

BOOL EnemyCheckAlive(POINT pos) {
	if (bombMap[pos.y][pos.x] == '2')
		return FALSE;

	return TRUE;
}