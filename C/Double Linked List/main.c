#include "DoubleLinkedList.h"

int main() {
	List* list = Init();
	int data;

	for (int i = 1; i < 9; i++)
		Insert(list, i);

	if (First(list, &data)) {
		printf("%d ", data);
		
		while (Next(list, &data))
			printf("%d ", data);

		while (Previous(list, &data))
			printf("%d ", data);

		puts("\n");
	}

	Release(list);
	return 0;
}