#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <process.h>

#define SAFE_FREE(x) x = NULL; free(x)

HINSTANCE m_hInstance;
HWND m_hWnd;

#endif