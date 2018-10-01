#include "List.h"

Node* Init(const int* const datas, const int numOfData) {
	Node* iter = malloc(sizeof(Node));
	Node* reVal = iter;

	for (int i = 0; i < numOfData; i++) {
		Insert(iter, datas[i]);
		iter = iter->next;
	}

	return reVal;
}

void Insert(Node* const head, const int data) {
	Node* newNode = malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->data = data;

	head->next = newNode;
}

void ForEach(Node* iter, void(*behavior)(Node*)) {
	iter = iter->next;

	while (iter->next != NULL) {
		Node* save = iter->next;
		behavior(iter);
		iter = save;
	}
}

void Print(Node *const node) {
	printf("%d ", node->data);
}

void SortEach(Node *const lhsIter) {
	Node *rhsIter, *minNode;
	rhsIter = minNode = lhsIter;

	while (rhsIter->next != NULL) {
		rhsIter = rhsIter->next;

		if (minNode->data > rhsIter->data)
			minNode = rhsIter;
	}

	Swap(lhsIter, minNode);
}

void Swap(Node *const lhs, Node *const rhs) {
	int temp = lhs->data;
	lhs->data = rhs->data;
	rhs->data = temp;
}

void SafeFree(Node *node) {
	free(node);
	node = NULL;
}