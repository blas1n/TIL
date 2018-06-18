#include <stdio.h>

int fibonacci(int n) {
	static int memo[41] = { 0 };
	if (memo[n] > 0) return memo[n];
	if (n == 0) return 0;
	if (n == 1 || n == 2) return 1;
	return memo[n] = fibonacci(n - 2) + fibonacci(n - 1);
}


int main() {
	int T, N;
	scanf("%d", &T);

	for (int i = 0; i < T; ++i) {
		scanf("%d", &N);
		if (N == 0) printf("1 0\n");
		else printf("%d %d\n", fibonacci(N - 1), fibonacci(N));
	}

	return 0;
}
