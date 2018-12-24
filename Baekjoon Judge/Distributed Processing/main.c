#include <stdio.h>

int pow(int base, int index, int mod) {
	int r = 1;

	for (; index; index >>= 1) {
		if (index & 1)
			r = (r * base) % mod;

		base = (base * base) % mod;
	}

	return r;
}

int main() {
	int n, a, b, r;
	scanf("%d", &n);

	while (n--) {
		scanf("%d %d", &a, &b);

		r = pow(a, b, 10);

		if (!r) r = 10;

		printf("%d\n", r);
	}

	return 0;
}