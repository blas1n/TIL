#ifndef __SINLIB_H__
#define __SINLIB_H__

#define _CRT_SUCURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>
#include <Windows.h>

typedef TCHAR* str;
typedef const TCHAR* cstr;
typedef unsigned int uint;
typedef int* intptr;
typedef float* floatptr;
typedef double* doubleptr;

#define SAFE_FREE(x) x = NULL; free(x);
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

void* Input(cstr guide, const int maxCount);
void Print(cstr guide);
void Println(cstr guide);
char GetKey();
void Scan(str buf, int maxCount, FILE* fp);
void InputFlush();
int Random(int area, int start);
void gotoxy(int posX, int posY);
void Delay(float cool);

#endif // !__SINLIB_H__
