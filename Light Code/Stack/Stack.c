#include "Stack.h"

void Push(Stack* self, int value) {
	if (++(self->top) >= self->data + self->capacity)
		return;

	*(self->top) = value;
}

int Pop(Stack* self) {
	if (--(self->top) < self->data)
		return -1;

	return *((self->top) + 1);
}

int Top(Stack* self) {
	return *(self->top);
}

void Print(Stack* self) {
	for (int* i = self->data; i < self->top; i++)
		printf("%d", *i);
}

void Clear(Stack* self) {
	self->top = 0;
}

Stack* InitStack(int capacity) {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->data = (int*)malloc(sizeof(int) * capacity);
	stack->top = stack->data;
	stack->capacity = capacity;
	
	stack->Push = Push;
	stack->Pop = Pop;
	stack->Top = Top;
	stack->Print = Print;
	stack->Clear = Clear;

	return stack;
}
void DeleteStack(Stack* stack) {
	SAFE_FREE(stack->data);
	SAFE_FREE(stack);
}