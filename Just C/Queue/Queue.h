typedef struct _Node {
	int data;
	struct _Node* next;
} Node;

typedef struct _Queue {
	Node* front;
	Node* rear;
	int count;
}Queue;

Queue* InitQueue();
void Release(Queue* self);

void Enqueue(Queue* self, int data);
int Dequeue(Queue* self);

int GetCount(Queue* self);