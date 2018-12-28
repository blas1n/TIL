#include <stdio.h>

#define RADIX 10

int main() {
	int radix[RADIX] = { 0, };

	int a, b, c, n;
	scanf("%d %d %d", &a, &b, &c);

	for (n = a * b * c; n > 0; n /= RADIX)
		radix[n % RADIX]++;

	for (int i = 0; i < RADIX; i++)
		printf("%d\n", radix[i]);

	return 0;
}