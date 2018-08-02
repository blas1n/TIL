#include "PlayerControl.h"

void Move(WPARAM key) {
	if (!isAlive) return;

	POINT newPos = playerPos;

	if (key == 'W' || key == 'w' || key == VK_UP) {
		if (cellMap[newPos.y - 1][newPos.x] != '0' || bombMap[newPos.y - 1][newPos.x] == '1')
			return;

		newPos.y--;
		PlayerMoveInCell(newPos);
	}

	else if (key == 'S' || key == 's' || key == VK_DOWN) {
		if (cellMap[newPos.y + 1][newPos.x] != '0' || bombMap[newPos.y + 1][newPos.x] == '1')
			return;

		newPos.y++;
		PlayerMoveInCell(newPos);
	}

	else if (key == 'A' || key == 'a' || key == VK_LEFT) {
		if (cellMap[newPos.y][newPos.x - 1] != '0' || bombMap[newPos.y][newPos.x - 1] == '1')
			return;

		newPos.x--;
		PlayerMoveInCell(newPos);
	}

	else if (key == 'D' || key == 'd' || key == VK_RIGHT) {
		if (cellMap[newPos.y][newPos.x + 1] != '0' || bombMap[newPos.y][newPos.x + 1] == '1')
			return;

		newPos.x++;
		PlayerMoveInCell(newPos);
	}
}

BOOL CheckAlive() {
	if (bombMap[playerPos.y][playerPos.x] == '2')
		isAlive = FALSE;
	
	return isAlive;
}