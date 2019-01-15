#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
	int data;
	struct _Node* next;
} Node;

Node* Init(const int *const datas, const int numOfData);
void Insert(Node *const head, const int data);
void ForEach(Node *iter, void(*behavior)(Node*));
void Print(Node *const node);
void SortEach(Node *const iter);
void Swap(Node *const lhs, Node *const rhs);
void SafeFree(Node *node);

#endif