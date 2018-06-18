#include <stdio.h>

int main() {
	int want, x = 64, count = 0;

	scanf("%d", &want);

	while (!(want == 0)) {
		if (!(x > want)) {
			want -= x;
			count++;
		}

		x >>= 1;
	}

	printf("%d\n", count);

	return 0;
}