#include "DoubleLinkedList.h"

List* Init() {
	List* list = (List*)malloc(sizeof(List));

	memset(list, 0, sizeof(List));
	return list;
}

void Release(List* list) {
	Node* save;

	if (First(list, NULL)) {

		save = list->cur->next;
		free(list->cur);
		list->cur = NULL;

		while (Next(list, NULL)) {
			save = list->cur->next;
			free(list->cur);
			list->cur = NULL;
		}
	}

	memset(list, 0, sizeof(List));

	free(list);
	list = NULL;
}

void Insert(List* list, int data) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;

	newNode->next = list->head;

	if (list->head)
		list->head->prev = newNode;


	newNode->prev = NULL;
	list->head = newNode;

	list->count++;
}

int First(List* list, int* data) {
	if (!list->head)
		return 0;

	list->cur = list->head;

	if (data)
		*data = list->cur->data;

	return 1;
}

int Next(List* list, int* data) {
	if (!list->cur->next)
		return 0;

	list->cur = list->cur->next;
	
	if (data)
		*data = list->cur->data;

	return 1;
}

int Previous(List* list, int* data) {
	if (!list->cur->prev)
		return 0;

	list->cur = list->cur->prev;
	
	
	if (data)
		*data = list->cur->data;

	return 1;
}

int Count(List* list) {
	return list->count;
}