#include <stdio.h>
#include <string.h>

int main() {
	char buf[100];

	fgets(buf, 100, stdin);

	for (int i = 0; i < strlen(buf); i++) {
		printf("%c", buf[i]);

		if (!((i + 1) % 10)) puts("");
	}

	return 0;
}