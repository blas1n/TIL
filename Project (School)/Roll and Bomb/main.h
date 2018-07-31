#ifndef __MAIN_H__
#define __MAIN_H__

#include "common.h"
#include "Renderer.h"
#include "KeyControl.h"
#include "MapManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void Init();
void Release();

#endif