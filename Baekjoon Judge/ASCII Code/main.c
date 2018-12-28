#include <stdio.h>
#include <stdlib.h>

int Compare(const void* pLhs, const void* pRhs) {
	int lhs = *(int*)pLhs;
	int rhs = *(int*)pRhs;

	if (lhs < rhs)
		return -1;

	if (lhs > rhs)
		return 1;

	return 0;
}

int main() {
	int n;
	scanf("%d", &n);

	int* nums = malloc(n * sizeof(int));

	for (int i = 0; i < n; i++)
		scanf("%d", nums + i);

	qsort(nums, n, sizeof(int), Compare);

	for (int i = 0; i < n; i++)
		printf("%d\n", nums[i]);

	return 0;
}