#include "Stack.h"

int main() {
	Stack* stack = InitStack(20);

	stack->Push(stack, 5);
	stack->Push(stack, 6);
	stack->Print(stack);
	for (int i = 3; i > 0; i--) {
		int result = stack->Pop(stack);

		if (!result) { continue; }
		printf("%d\n", result);
	}

	DeleteStack(stack);
	return 0;
}