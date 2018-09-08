#include <stdio.h>

int main() {
	int n, lhs, rhs;
	scanf("%d", &n);

	while (n--) {
		scanf("%d %d", &lhs, &rhs);
		printf("%d\n", lhs + rhs);
	}

	return 0;
}