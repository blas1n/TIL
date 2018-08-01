#ifndef __BOMBMANAGER_H__
#define __BOMBMANAGER_H__

#include "common.h"
#include "MapManager.h"
#include <time.h>

#pragma comment(lib, "winmm.lib")

void InstallBomb(POINT);
void WINAPI BurstBomb(LPVOID);

#endif