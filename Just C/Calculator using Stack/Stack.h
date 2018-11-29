#pragma once

typedef struct _Node {
	int data;
	struct _Node* next;
} Node;

typedef struct _Stack {
	Node* head;
} Stack;

Stack* InitStack();
int IsEmpty(Stack* stack);

void Push(Stack* stack, int data);
int Pop(Stack* stack);
int Peek(Stack* stack);