#include <stdio.h>
#include <stdlib.h>

int main() {
	int n, result = 0;
	scanf("%d", &n);
	getchar();

	char* buf = (char*)malloc(n);
	fgets(buf, n, stdin);

	for (int i = 0; i < n; i++) {
		result += atoi(&buf[i]);
	}

	printf("%d\n", result);
	free(buf);
	buf = NULL;
	return 0;
}