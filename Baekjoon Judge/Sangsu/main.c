#include <stdio.h>

int max(int lhs, int rhs) {
	return lhs > rhs ? lhs : rhs;
}

int main() {
	int a, b, ra = 0, rb = 0;
	scanf("%d %d", &a, &b);

	for (int i = 100; i > 0; i /= 10) {
		ra += (a % 10) * i;
		rb += (b % 10) * i;

		a /= 10;
		b /= 10;
	}

	printf("%d\n", max(ra, rb));
	return 0;
}