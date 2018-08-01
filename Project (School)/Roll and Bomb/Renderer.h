#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "common.h"
#include "MapManager.h"

RECT winRect;

void Render(HDC);
void RenderCell(HDC);
void RenderBomb(HDC);

#endif