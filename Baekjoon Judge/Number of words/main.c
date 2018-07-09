#include <stdio.h>
#include <stdlib.h>

int main() {
	int count = 0, i = 0;

	char* input = (char*)calloc(100, sizeof(char));
	fgets(input, 100, stdin);

	do {
		if (input[i] == ' ')
			count++; 7
	} while (input[i]++ != '\n');

	free(input);
	printf("%d", count + 1);
	return 0;
}