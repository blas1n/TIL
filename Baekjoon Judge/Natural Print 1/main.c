#include <stdio.h>

int main() {
	char a = 0;
	while ((a = getchar()) != -1)
		putchar(a);
}
