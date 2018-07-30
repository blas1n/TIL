#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <stdio.h>
#include <Windows.h>

int nowStage;
SIZE mapSize;
char* cellMap;

void Init();
void Render(HDC, RECT);
void Release();

void MapRead();

#endif