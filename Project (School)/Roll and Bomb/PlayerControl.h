#ifndef __PLAYERCONTROL_H__
#define __PLAYERCONTROL_H__

#include "common.h"
#include "MapManager.h"
#include "BombManager.h"

void Move(WPARAM);
void Attack();
BOOL CheckAlive();

BOOL isAlive;

#endif