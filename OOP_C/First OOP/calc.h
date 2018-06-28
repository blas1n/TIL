#ifndef __CALC_H__
#define __CALC_H__

#define SAFE_FREE(x) x = NULL; free(x);

#define class struct

typedef class Calc {
	int(*Add) (int, int);
	int(*Sub) (int, int);
	int(*Mul) (int, int);
	int(*Div) (int, int);
} Calc;

Calc* NewCalc();
Calc* DeleteCalc(Calc* calc);

int Add(int a, int b);
int Sub(int a, int b);
int Mul(int a, int b);
int Div(int a, int b);

#endif