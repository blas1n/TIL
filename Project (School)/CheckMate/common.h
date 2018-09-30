#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include <process.h>

#pragma warning (disable:4996)

#define SAFE_FREE(x) free(x); x = NULL 

HINSTANCE m_hInstance;
HWND m_hWnd;

#endif