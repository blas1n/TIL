#include "Queue.h"

int main() {
	Queue* queue = InitQueue(20);

	queue->Enqueue(queue, 5);
	queue->Enqueue(queue, 6);
	queue->Print(queue);
	for (int i = 3; i > 0; i--) {
		int result = queue->Dequeue(queue);

		if (!result) { continue; }
		printf("%d\n", result);
	}

	DeleteQueue(queue);
	return 0;
}