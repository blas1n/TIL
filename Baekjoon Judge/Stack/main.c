#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _NODE {
	int data;
	struct _NODE* next;
} Node;

typedef struct _STACK {
	Node* head;
	int count;
} Stack;

void Init(Stack* pStack);
void Release(Stack* pStack);
void Push(Stack* pStack, int data);
int Pop(Stack* pStack);
int Top(Stack* pStack);
int Size(Stack* pStack);
int Empty(Stack* pStack);

int main() {
	Stack* pStack = malloc(sizeof(Stack));
	Init(pStack);

	int n, data;
	scanf("%d", &n);

	char buffer[10];

	while (n--) {
		scanf("%s", buffer);

		if (!strcmp(buffer, "push")) {
			scanf("%d", &data);
			Push(pStack, data);
			continue;
		}

		if (!strcmp(buffer, "pop")) {
			printf("%d\n", Pop(pStack));
			continue;
		}

		if (!strcmp(buffer, "top")) {
			printf("%d\n", Top(pStack));
			continue;
		}

		if (!strcmp(buffer, "size")) {
			printf("%d\n", Size(pStack));
			continue;
		}

		if (!strcmp(buffer, "empty")) {
			printf("%d\n", Empty(pStack));
			continue;
		}
	}

	Release(pStack);
	free(pStack);
	pStack = NULL;

	return 0;
}

void Init(Stack* pStack) {
	pStack->head = NULL;
	pStack->count = 0;
}

void Release(Stack* pStack) {
	while (Pop(pStack) != -1);
}

void Push(Stack* pStack, int data) {
	Node* newNode = malloc(sizeof(Node));
	newNode->data = data;

	newNode->next = pStack->head;
	pStack->head = newNode;

	pStack->count++;
}

int Pop(Stack* pStack) {
	if (!pStack->head) return -1;

	int reVal = pStack->head->data;
	
	Node* newHead = pStack->head->next;

	free(pStack->head);
	pStack->head = newHead;

	pStack->count--;

	return reVal;
}

int Top(Stack* pStack) {
	if (!pStack->head) return -1;

	return pStack->head->data;
}

int Size(Stack* pStack) {
	return pStack->count;
}

int Empty(Stack* pStack) {
	return Size(pStack) == 0;
}