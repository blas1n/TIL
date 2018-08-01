#include "PlayerControl.h"

void Move(WPARAM key) {
	if (!isAlive) return;

	POINT newPos = playerPos;

	switch (key) {
	case 'W':
		if (cellMap[newPos.y - 1][newPos.x] != '0')
			break;

		newPos.y--;
		PlayerMoveInCell(newPos);
		break;

	case 'S':
		if (cellMap[newPos.y + 1][newPos.x] != '0')
			break;

		newPos.y++;
		PlayerMoveInCell(newPos);
		break;

	case 'A':
		if (cellMap[newPos.y][newPos.x - 1] != '0')
			break;

		newPos.x--;
		PlayerMoveInCell(newPos);
		break;

	case 'D':
		if (cellMap[newPos.y][newPos.x + 1] != '0')
			break;

		newPos.x++;
		PlayerMoveInCell(newPos);
		break;
	}
}

void Attack() {
	if (!isAlive) return;

	InstallBomb(playerPos);
}

BOOL CheckAlive() {
	if (bombMap[playerPos.y][playerPos.x] == '2')
		isAlive = FALSE;
	
	return isAlive;
}