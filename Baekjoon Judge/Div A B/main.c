#include <stdio.h>

int main() {
	double a, b;
	double re;
	scanf("%lf %lf", &a, &b);
	re = a / b;
	printf("%.9lf", re);

	return 0;
}