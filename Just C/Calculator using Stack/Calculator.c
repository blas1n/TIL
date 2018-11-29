#include "Calculator.h"

#include <ctype.h>
#include <string.h>
#include "Stack.h"

int EvalRPNExp(char* exp) {
	Stack* stack = InitStack();
	char tok, op1, op2;

	for (int i = 0; i < strlen(exp); i++) {
		tok = exp[i];

		if (isdigit(tok)) Push(stack, tok - '0');

		else {
			op2 = Pop(stack);
			op1 = Pop(stack);

			switch (tok) {
			case '+':
				Push(stack, op1 + op2);
				break;
				
			case '-':
				Push(stack, op1 - op2);
				break;

			case '*':
				Push(stack, op1 * op2);
				break;

			case '/':
				Push(stack, op1 / op2);
				break;
			}
		}
	}

	return Pop(stack);
}