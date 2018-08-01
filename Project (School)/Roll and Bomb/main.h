#ifndef __MAIN_H__
#define __MAIN_H__

#include "common.h"
#include "Renderer.h"
#include "PlayerControl.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Init();
void Release();
BOOL Update();

#endif