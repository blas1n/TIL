#include "Queue.h"
#include <memory.h>

void Enqueue(Queue* self, int value) {
	if (*(self->data) == 0) {
		*(self->head) = value;
		return;
	}

	if (self->head + 1 >= self->data + self->capacity)
		self->head = self->data;

	*(++(self->head)) = value;
}

int Dequeue(Queue* self) {
	if (self->tail + 1 >= self->data + self->capacity)
		self->tail = self->data;

	if (!(*(self->tail)))
		perror("더 이상 뺄 수 있는 데이터가 없습니다");

	else
		return *((self->tail)++);

	return 0;
}

int Top(Queue* self) {
	return *(self->head);
}

void Print(Queue* self) {
	for (int* i = self->data; i <= self->head; i++)
		printf("%d\n", *i);
}

void Clear(Queue* self) {
	self->head = self->data;
	self->tail = self->data;
}

Queue* InitQueue(int capacity) {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->data = (int*)malloc(sizeof(int) * capacity);
	queue->head = queue->data;
	queue->tail = queue->data;
	queue->capacity = capacity;

	memset(queue->data, 0, capacity);

	queue->Enqueue = Enqueue;
	queue->Dequeue = Dequeue;
	queue->Top = Top;
	queue->Print = Print;
	queue->Clear = Clear;

	return queue;
}

void DeleteQueue(Queue* queue) {
	SAFE_FREE(queue->data);
	SAFE_FREE(queue);
}