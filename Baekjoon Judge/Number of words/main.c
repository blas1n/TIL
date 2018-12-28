#include <stdio.h>
#include <stdlib.h>

int main() {
	int count = 0;
	int isSplit = 1;

	char* input = malloc(1000000 * sizeof(char));
	scanf("%[^\n]s", input);

	for (int i = 0; input[i]; i++) {
		if (input[i] == ' ')
			isSplit = 1;

		else if (isSplit) {
			count++;
			isSplit = 0;
		}
	}

	free(input);
	input = NULL;

	printf("%d\n", count);
	return 0;
}