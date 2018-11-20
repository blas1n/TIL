#include "Queue.h"
#include <stdlib.h>
#include <memory.h>

Queue* InitQueue() {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	memset(queue, 0, sizeof(Queue));
}

void Release(Queue* self) {
	while (Dequeue(self));

	free(self);
	self = NULL;
}

void Enqueue(Queue* self, int data) {
	Node* newNode = (Node*)malloc(sizeof(Node));

	newNode->data = data;
	newNode->next = NULL;

	if (!GetCount(self))
		self->front = newNode;
	
	else
		self->rear->next = newNode;
	
	self->rear = newNode;
	self->count++;
}

int Dequeue(Queue* self) {
	Node* node;
	int reVal = 0;

	if (!GetCount(self))
		return reVal;
	
	node = self->front;
	reVal = node->data;

	self->front = node->next;

	free(self);
	self = NULL;

	self->count--;

	return reVal;
}

int GetCount(Queue* self) {
	return self->count;
}