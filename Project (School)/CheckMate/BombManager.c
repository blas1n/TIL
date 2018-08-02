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

	SAFE_FREE(installPos);
	
	Sleep(1500);

	// 300초 동안 폭발을 유지했다가 폭발이 사라진다.
	for (DWORD startTime = timeGetTime(); 500 > timeGetTime() - startTime; ) {
		for (int y = bombPos.y - 1; y <= bombPos.y + 1; y++) {
			for (int x = bombPos.x - 1; x <= bombPos.x + 1; x++) {
				if (cellMap[y][x] != '1')
					bombMap[y][x] = '2';
			}
		}
	}

	for (int y = bombPos.y - 1; y <= bombPos.y + 1; y++) {
		for (int x = bombPos.x - 1; x <= bombPos.x + 1; x++)
			bombMap[y][x] = '0';
	}
}