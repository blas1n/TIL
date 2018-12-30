#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int Min(int lhs, int rhs) {
	return lhs < rhs ? lhs : rhs;
}

int main() {
	int n, i, d[1001][3] = { 0, }, a[1001][3] = { 0, };
	scanf("%d", &n);

	for (i = 1; i <= n; i++)
		scanf("%d %d %d", &a[i][0], &a[i][1], &a[i][2]);

	for (i = 1; i <= n; i++) {
		d[i][0] = Min(d[i - 1][1], d[i - 1][2]) + a[i][0];
		d[i][1] = Min(d[i - 1][0], d[i - 1][2]) + a[i][1];
		d[i][2] = Min(d[i - 1][0], d[i - 1][1]) + a[i][2];
	}

	printf("%d\n", Min(Min(d[n][0], d[n][1]), d[n][2]));
	
	return 0;
}