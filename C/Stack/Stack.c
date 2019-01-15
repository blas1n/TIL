#include "Stack.h"
#include <memory.h>

void Push(Stack* self, int value) {
	if (self->top + 1 >= self->data + self->capacity)
		puts("용량이 부족합니다");

	else
		*(++self->top) = value;
}

int Pop(Stack* self) {
	if (self->top < self->data)
		puts("더 이상 뺄 수 있는 데이터가 없습니다");

	else return *((self->top)--);

	return -1;
}

int Peek(Stack* self) {
	if (self->top < self->data)
		puts("데이터가 없습니다");

	else return *(self->top);

	return -1;
}

void Print(Stack* self) {
	for (int* i = self->data; i <= self->top; i++)
		printf("%d\n", *i);
}

void Clear(Stack* self) {
	self->top = self->data - 1;
}

Stack* InitStack(int capacity) {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->data = (int*)malloc(sizeof(int) * capacity);
	stack->top = stack->data - 1;
	stack->capacity = capacity;

	memset(stack->data, 0, capacity);
	
	stack->Push = Push;
	stack->Pop = Pop;
	stack->Peek = Peek;
	stack->Print = Print;
	stack->Clear = Clear;

	return stack;
}

void DeleteStack(Stack* stack) {
	SAFE_FREE(stack->data);
	SAFE_FREE(stack);
}