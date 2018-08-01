#ifndef __MAPMANAGER_H__
#define __MAPMANAGER_H__

#include "common.h"

char** cellMap;
char** bombMap;

SIZE mapSize;
int nowStage;

POINT playerPos;
POINT beforePos;

void MapRead();
SIZE GetSize(FILE*);
void MapDataFree();
void PlayerMoveInCell(POINT);

#endif