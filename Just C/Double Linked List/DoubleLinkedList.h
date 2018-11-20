#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct _Node {
	int data;
	struct _Node* next;
	struct _Node* prev;
} Node;

typedef struct _DoubleLinkedList {
	Node* head;
	Node* cur;
	int count;
} List;

List* Init();
void Release(List* list);

void Insert(List* list, int data);
int Count(List* list);

int First(List* list, int* data);
int Next(List* list, int* data);
int Previous(List* list, int* data);