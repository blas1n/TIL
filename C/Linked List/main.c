#include "List.h"

int main() {
	const int datas[12] = { 16, 32, 23, 61, 75, 57, 83, 38, 97, 79, 47, 41 };
	Node* head = Init(datas, sizeof(datas) / sizeof(datas[0]));

	ForEach(head, Print);
	puts("");

	ForEach(head, SortEach);

	ForEach(head, Print);
	puts("");

	ForEach(head, SafeFree);
	return 0;
}