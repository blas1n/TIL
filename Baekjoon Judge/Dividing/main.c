#include <stdio.h>

int main() {
	int n, f, low;
	low = 0;

	scanf("%d %d", &n, &f);
	n /= 100;

	while ((n * 100 + low) % f) {
		if (low >= 100)
			return 0;

		low++;
	}

	printf("%02d\n", low);

	return 0;
}