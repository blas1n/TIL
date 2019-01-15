#include <string.h>
#include <stdlib.h>
#include "InfixToPostfix.h"
#include "Calculator.h"
#include "InfixCalculator.h"

int EvalInfixExp(char exp[]) {
	int len = strlen(exp);
	int ret;
	char* expcpy = malloc(len + 1);
	strcpy(expcpy, exp);

	ConvToPRNExp(expcpy);
	ret = EvalRPNExp(expcpy);

	free(expcpy);
	return ret;
}