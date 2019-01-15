#include "common.h"

char* Input(const char* guide) {
	char* result = (char*)malloc(MAX_LEN * sizeof(char));

	fputs(guide, stdout);
	fgets(result, MAX_LEN, stdin);

	result[strlen(result + 1)] = '\0';

	return result;
}