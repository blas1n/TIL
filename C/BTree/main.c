#include "BTree.h"

int main() {
	Node* head = InitNode(1);

	SetLeftNode(head, InitNode(2));
	SetRightNode(head, InitNode(3));

	SetLeftNode(GetLeftNode(head), InitNode(4));
	SetRightNode(GetLeftNode(head), InitNode(5));

	SetLeftNode(GetRightNode(head), InitNode(6));
	SetRightNode(GetRightNode(head), InitNode(7));

	InOrderTraverse(head);
}