#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* InitStack() {
	Stack* stack = malloc(sizeof(Stack));
	stack->head = NULL;

	return stack;
}

int IsEmpty(Stack* stack) {
	return !stack->head;
}

void Push(Stack* stack, int data) {
	Node* node = (Node*)malloc(sizeof(Node));

	node->data = data;
	node->next = stack->head;

	stack->head = node;
}

int Pop(Stack* stack) {
	int data;
	Node* node;

	if (IsEmpty(stack)) {
		return -1;
	}

	data = stack->head->data;
	node = stack->head;

	stack->head = stack->head->next;
	free(node);

	return data;
}

int Peek(Stack* stack) {
	if (IsEmpty(stack)) {
		return -1;
	}

	return stack->head->data;
}