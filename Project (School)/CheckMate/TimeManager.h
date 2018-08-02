#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include "common.h"
#include <math.h>

typedef struct tagTime {
	short minute;
	short second;
} Time;

Time takeTime;

Time GetTakeTime();
void WINAPI IncreaseTime();

#endif