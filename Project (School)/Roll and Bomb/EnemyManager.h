#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "common.h"
#include "MapManager.h"

enum { FRONT, BACK, LEFT, RIGHT };

void WINAPI AI(LPVOID);
BOOL EnemyCheckAlive(POINT pos);

#endif