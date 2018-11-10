#include "BombManager.h"

void InstallBomb(POINT bombPos) {
	bombMap[bombPos.y][bombPos.x] = '1';

	POINT* installPos = (POINT*)malloc(sizeof(POINT));
	installPos->x = bombPos.x;
	installPos->y = bombPos.y;

	HANDLE bombThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)BurstBomb, (LPVOID)installPos, 0, NULL);
	CloseHandle(bombThread);
}

void WINAPI BurstBomb(LPVOID installPos) {
	POINT bombPos = *((POINT*)installPos);
	char oldObject[3][3];
	memset(oldObject, '0', sizeof(oldObject));

	SAFE_FREE(installPos);
	
	Sleep(1500);

	// 폭탄을 제외하고 폭탄이 터질 위치에 있던 물체를 기억한다.
	for (int y = bombPos.y - 1; y <= bombPos.y + 1; y++) {
		for (int x = bombPos.x - 1; x <= bombPos.x + 1; x++) {
			if (x == bombPos.x && y == bombPos.y) continue;

			if (bombMap[y][x] != '2')
				oldObject[y - bombPos.y + 1][x - bombPos.x + 1] = bombMap[y][x];
		}
	}

	// 300초 동안 폭발을 유지했다가 폭발이 사라진다.
	for (DWORD startTime = timeGetTime(); 350 > timeGetTime() - startTime; ) {
		for (int y = bombPos.y - 1; y <= bombPos.y + 1; y++) {
			for (int x = bombPos.x - 1; x <= bombPos.x + 1; x++) {
				if (cellMap[y][x] != '1')
					bombMap[y][x] = '2';
			}
		}
	}

	// 물체를 되돌린다.
	for (int y = bombPos.y - 1; y <= bombPos.y + 1; y++) {
		for (int x = bombPos.x - 1; x <= bombPos.x + 1; x++)
			bombMap[y][x] = oldObject[y - bombPos.y + 1][x - bombPos.x + 1];
	}
}