#include "Stack.h"

int main() {
	Stack* stack = InitStack(20);

	stack->Push(stack, 5);
	stack->Push(stack, 6);
	printf("%d", stack->Pop(stack));
	printf("%d", stack->Pop(stack));

	DeleteStack(stack);
	return 0;
}