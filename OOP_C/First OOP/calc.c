#include "calc.h"
#include <stdlib.h>

Calc* NewCalc() {
	Calc* calc = (Calc*)malloc(sizeof(Calc));

	calc->Add = Add;
	calc->Sub = Sub;
	calc->Mul = Mul;
	calc->Div = Div;

	return calc;
}

Calc* DeleteCalc(Calc* calc) {
	SAFE_FREE(calc);
}

int Add(int a, int b) {
	return a + b;
}

int Sub(int a, int b) {
	return a - b;
}

int Mul(int a, int b) {
	return a * b;
}

int Div(int a, int b) {
	return a / b;
}