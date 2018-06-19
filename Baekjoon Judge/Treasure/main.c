#include <stdio.h>
#include <stdlib.h>

int* input(int* a, const int n);
void Sort(int* vector);
void Swap(int* vector, int a, int b);
int GetResult(int* a, int* b);

int n;

int main() {
	int i, limit;
	int *a = NULL, *b = NULL;

	scanf("%d", &n);
	a = input(a, n);
	b = input(b, n);

	Sort(a);
	Sort(b);

	limit = n & 1 ? n - 1 : n;
	limit /= 2;

	for (i = 0; i < limit; i++)
		Swap(b, i, n - 1 - i);

	printf("%d", GetResult(a, b));

	return 0;
}

int* input(int* vector, const int n) {
	vector = (int*)calloc(n, sizeof(int));

	for (int i = 0; i < n; i++)
		scanf("%d\n", &vector[i]);

	return vector;
}

void Sort(int* vector) {
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			if (vector[i] > vector[j])
				Swap(vector, i, j);
}

void Swap(int* vector, int a, int b) {
	vector[a] ^= vector[b];
	vector[b] ^= vector[a];
	vector[a] ^= vector[b];
}

int GetResult(int* a, int* b) {
	int result = 0;

	int* Add = (int*)calloc(n, sizeof(int));

	for (int i = 0; i < n; i++) {
		Add[i] = a[i] * b[i];
		result += Add[i];
	}

	return result;
}