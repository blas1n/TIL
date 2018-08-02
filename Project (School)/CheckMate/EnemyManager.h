#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "common.h"
#include "MapManager.h"
#include "BombManager.h"
#include "TimeManager.h"

enum { FRONT, BACK, LEFT, RIGHT };

void WINAPI AI(LPVOID);
BOOL EnemyCheckAlive(POINT);
void EnemyAttack(POINT);

BOOL bEnd;

#endif