#include "calc.h"
#include <stdio.h>

int main() {
	Calc* C = NewCalc();
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%d\n", C->Add(a, b));
	DeleteCalc(C);

	return 0;
}