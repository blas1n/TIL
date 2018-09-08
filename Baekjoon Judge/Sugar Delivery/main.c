#include <stdio.h>

int main() {
	int n, count = 0;
	scanf("%d", &n);

	while (n > 0) {
		if (n % 5 == 0)
			n -= 5;

		else
			n -= 3;

		count++;
	}

	if (n != 0) count = 0;

	printf("%d", count ? count : -1);
}