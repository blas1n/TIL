#ifndef __MAPMANAGER_H__
#define __MAPMANAGER_H__

#include "common.h"
#include "EnemyManager.h"
#include "Renderer.h"

char** cellMap;
char** bombMap;

SIZE mapSize;
int nowStage;
POINT playerPos;

int nowKill;
BOOL bClear;
BOOL mapLoad;

void MapRead();
SIZE GetSize(FILE*);
void MapDataFree();
void PlayerMoveInCell(POINT);
void NextMap();

#endif