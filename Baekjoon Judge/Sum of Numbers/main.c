#include <stdio.h>

int main() {
	char buf;

	int n, result = 0;
	scanf("%d", &n);
	getchar();

	for (int i = 0; i < n; i++) {
		buf = getchar();
		result += buf - '0';
	}

	printf("%d\n", result);
	return 0;
}