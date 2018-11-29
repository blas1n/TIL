#include "BTree.h"

Node* InitNode(int data) {
	Node* node = malloc(sizeof(Node));
	node->left = node->right = NULL;
	node->data = data;

	return node;
}

void FreeNode(Node* node) {
	free(node);
	node = NULL;
}

int GetNodeData(const Node* const node) {
	return node->data;
}

void SetNodeData(Node* node, int data) {
	node->data = data;
}

void SetLeftNode(Node* main, Node* sub) {
	if (main->left) FreeNode(main->left);
	main->left = sub;
}

void SetRightNode(Node* main, Node* sub) {
	if (main->right) FreeNode(main->right);
	main->right = sub;
}

Node* GetLeftNode(const Node* const node) {
	return node->left;
}

Node* GetRightNode(const Node* const node) {
	return node->right;
}

void InOrderTraverse(Node* node) {
	if (!node) return;

	InOrderTraverse(node->left);
	printf("%d\n", node->data);
	InOrderTraverse(node->right);
}