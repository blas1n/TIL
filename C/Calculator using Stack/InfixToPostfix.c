#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Stack.h"
#include "InfixToPostfix.h"

int GetOpPrec(char op) {
	switch (op) {
	case '*':
	case '/':
		return 5;

	case'+':
	case'-':
		return 3;

	case '(':
		return 1;
	}

	return -1;
}

int WhoPrecOp(char op1, char op2) {
	int op1Prec = GetOpPrec(op1);
	int op2Prec = GetOpPrec(op2);

	if (op1Prec > op2Prec)
		return 1;

	else if (op1Prec < op2Prec)
		return -1;

	else return 0;
}

void ConvToPRNExp(char* exp) {
	Stack* stack = InitStack();
	int expLen = strlen(exp);
	char* convExp = malloc(expLen + 1);

	int idx = 0;
	char tok, popOp;

	memset(convExp, 0, expLen + 1);
	
	for (int i = 0; i < expLen; i++) {
		tok = exp[i];
		if (isdigit(tok)) convExp[idx++] = tok;
		
		else {
			switch (tok) {
			case '(':
				Push(stack, tok);
				break;

			case ')':
				while (1) {
					popOp = Pop(stack);

					if (popOp == '(') break;

					convExp[idx++] = popOp;
				}
				break;

			case '+': case '-': case '*': case'/':
				while (!IsEmpty(stack) && WhoPrecOp(Peek(stack), tok) >= 0)
					convExp[idx++] = Pop(stack);

				Push(stack, tok);
				break;
			}
		}
	}

	while (!IsEmpty(stack))
		convExp[idx++] = Pop(stack);

	strcpy(exp, convExp);
	free(convExp);
}