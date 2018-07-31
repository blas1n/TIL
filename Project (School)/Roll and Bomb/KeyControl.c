#include "KeyControl.h"

void GetKey(WPARAM key) {
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