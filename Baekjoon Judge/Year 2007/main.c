#include <stdio.h>

int main() {
	char week[7][4] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
	int month, day, count = 0;
	scanf("%d %d", &month, &day);

	for (int i = 1; i < month; i++) {
		if (i == 4 || i == 6 || i == 9 || i == 11)
			count += 30;
		else if (i == 2)
			count += 28;
		else
			count += 31;
	}

	for (int i = 1; i <= day; i++)
		count++;

	printf("%s", week[count % 7]);
}