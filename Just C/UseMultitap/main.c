#include <stdio.h>
#include <stdlib.h>

int main()
{
	int multitap, i;
	int result = 0;

	scanf("%d", &multitap);
	int* concent = (int*)malloc(sizeof(int) * multitap);
	for (i = 0; i < multitap; i++) {
		scanf("%d", concent + i);
	}

	for (i = 0; i < multitap; i++) {
		result += *(concent + i);
	}
	free(concent);

	result -= (multitap - 1);

	printf("%d\n", result);
	return 0;
}