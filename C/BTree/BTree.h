#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
	int data;
	struct _Node* left;
	struct _Node* right;
} Node;

Node* InitNode(int data);
void FreeNode(Node* node);

int GetNodeData(const Node* const node);
void SetNodeData(Node* node, int data);

void SetLeftNode(Node* main, Node* sub);
void SetRightNode(Node* main, Node* sub);

Node* GetLeftNode(const Node* const node);
Node* GetRightNode(const Node* const node);

void InOrderTraverse(Node* node);