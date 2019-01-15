#include "Stack.h"

int main() {
	Stack* stack = InitStack(20);
	char* command = (char*)malloc(20);
	int buf;

	while (1) {
		scanf("%s", command);

		if (!strcmp(command, "push")) {
			scanf("%d", &buf);
			stack->Push(stack, buf);
		}

		else if (!strcmp(command, "pop")) {
			buf = stack->Pop(stack);

			if (buf != -1) printf("%d\n", buf);
		}

		else if (!strcmp(command, "peek")) {
			buf = stack->Peek(stack);

			if (buf != -1) printf("%d\n", buf);
		}

		else if (!strcmp(command, "print")) {
			stack->Print(stack);
		}

		else if (!strcmp(command, "clear")) {
			stack->Clear(stack);
		}
	}

	DeleteStack(stack);
	return 0;
}