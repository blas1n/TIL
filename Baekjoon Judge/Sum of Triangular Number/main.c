#include <stdio.h>

int GetT(int n);
int GetW(int k);

int main() {
	int tastCount, n;
	scanf("%d", &tastCount);

	while (tastCount--) {
		scanf("%d", &n);

		printf("%d\n", GetW(n));
	}

	return 0;
}

int GetT(int n) {
	return (n * (n + 1)) / 2;
}

int GetW(int n) {
	int result = 0;

	for (int k = 1; k <= n; k++)
		result += k * GetT(k + 1);

	return result;
}