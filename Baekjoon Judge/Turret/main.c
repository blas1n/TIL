#include <stdio.h>
#include <math.h>

int main() {
	int t, x1, x2, y1, y2, r1, r2;
	double i;

	scanf("%d", &t);
	
	for (; t > 0; t--) {
		scanf("%d %d %d %d %d %d", &x1, &y1, &r1, &x2, &y2, &r2);

		if (x1 == x2 && y1 == y2) {
			if (r1 == r2) {
				printf("-1\n");
			}
			else {
				printf("0\n");
			}
		}

		else {
			i = sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));

			if ((r1 + r2) > i && abs(r1 - r2) < i) {
				printf("2\n");
			}
			else if ((r1 + r2) == i || abs(r1 - r2) == i) {
				printf("1\n");
			}
			else {
				printf("0\n");
			}
		}
	}

	return 0;
}