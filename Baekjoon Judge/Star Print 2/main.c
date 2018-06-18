#include <stdio.h>

int main() {
	int n;

	scanf("%d", &n);

	for (int limit = 1; limit <= n; limit++) {
		for (int i = 0; i < n - limit; i++)
			printf(" ");
		for (int i = 0; i < limit; i++)
			printf("*");
		printf("\n");
	}

	return 0;
}