#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_FREE(x) (x) = NULL; free(x)

typedef struct STACK {
	int* data;
	int* top;
	int capacity;

	void(*Push)(struct STACK*, int);
	int(*Pop)(struct STACK*);
	int(*Top)(struct STACK*);
	void(*Print)(struct STACK*);
	void(*Clear)(struct STACK*);
} Stack;

void Push(Stack* self, int value);
int Pop(Stack* self);
int Top(Stack* self);
void Print(Stack* self);
void Clear(Stack* self);

Stack* InitStack(int capacity);
void DeleteStack(Stack* stack);

#endif