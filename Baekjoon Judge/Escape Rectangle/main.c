#include <stdio.h>

int main() {
	int x, y, w, h, min;
	scanf("%d %d %d %d", &x, &y, &w, &h);

	w -= x;
	h -= y;

	w = w < x ? w : x;
	h = h < y ? h : y;

	min = w < h ? w : h;
	printf("%d\n", min);
	return 0;
}