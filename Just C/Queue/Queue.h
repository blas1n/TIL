#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>

#define SAFE_FREE(x) (x) = NULL; free(x)

typedef struct QUEUE {
	int* data;
	int* head;
	int* tail;
	int capacity;

	void(*Enqueue)(struct QUEUE*, int);
	int(*Dequeue)(struct QUEUE*);
	int(*Top)(struct QUEUE*);
	void(*Print)(struct QUEUE*);
	void(*Clear)(struct QUEUE*);
} Queue;

void Enqueue(Queue* self, int value);
int Dequeue(Queue* self);
int Top(Queue* self);
void Print(Queue* self);
void Clear(Queue* self);

Queue* InitQueue(int capacity);
void DeleteQueue(Queue* queue);

#endif