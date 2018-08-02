#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "common.h"
#include "MapManager.h"
#include "TimeManager.h"

RECT winRect;

void Render(HDC);
void RenderCell(HDC);
void RenderBomb(HDC);
void RenderDeath(HDC);
void RenderLoad(HDC);
void RenderStart(HDC);
void RenderTimer(HDC);
void RenderClear(HDC);

#endif