#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stdio.h>
#include <Windows.h>

int nowStage;
int mapSize;
char* cellMap;

void Init();
void Render(HDC hDC);
void Release();

#endif