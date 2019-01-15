#include "AVLTree.h"
#include <iostream>

AVLNode* AVLTree::Insert(AVLNode* root, const string& key, const string& value) {
	if (!root) {
		root = new AVLNode();
		
		root->key = key;
		root->value = value;

		root->left = root->right = nullptr;

		return root;
	}

	if (root->key == key) {
		std::cout << "존재하는 단어입니다." << std::endl;
		return root;
	}

	else if (root->key > key) {
		root->left = Insert(root->left, key, value);
		root = Balance(root);
	}

	else {
		root->right = Insert(root->right, key, value);
		root = Balance(root);
	}

	return root;
}

AVLNode* AVLTree::Search(AVLNode* root, const string& key) {
	if (!root)
		return nullptr;

	if (root->key == key)
		return root;

	else if (root->key > key)
		return Search(root->left, key);

	else
		return Search(root->right, key);
}

AVLNode* AVLTree::Delete(AVLNode*& root, const string& key) {
	if (!root)
		return nullptr;

	if (root->key == key) {
		AVLNode* reValue = new AVLNode(*root);
		AVLNode* temp = nullptr;

		if (root->left && !root->right)
			temp = new AVLNode(*(root->left));

		else if (root->right && !root->left)
			temp = new AVLNode(*(root->right));

		else if (root->left && root->right) {
			AVLNode* iter = root->right;

			while (iter->left) iter = iter->left;

			iter->left = root->left;

			temp = new AVLNode(*(root->right));
		}

		root->left = root->right = nullptr;

		delete root;
		root = temp;

		if (root) Balance(root);

		return reValue;
	}
		
	else if (root->key > key)
		return Delete(root->left, key);

	else
		return Delete(root->right, key);
}

void AVLTree::Inorder(AVLNode* root) {
	if (!root) return;

	Inorder(root->left);
	std::cout << root << std::endl;
	Inorder(root->right);
}

int AVLTree::Height(AVLNode* temp) {
	int h = 0;

	if (temp) {
		int left = Height(temp->left);
		int right = Height(temp->right);

		h = std::max(left, right) + 1;
	}

	return h;
}

int AVLTree::Diff(AVLNode* temp) {
	if (!temp) return 0;

	int left = Height(temp->left);
	int right = Height(temp->right);

	return left - right;
}

AVLNode* AVLTree::Balance(AVLNode* temp) {
	int factor = Diff(temp);

	if (factor > 1) {
		if (Diff(temp->left) > 0)
			temp = RotateLL(temp);

		else
			temp = RotateLR(temp);
	}

	if (factor < -1) {
		if (Diff(temp->right) > 0)
			temp = RotateRL(temp);

		else
			temp = RotateRR(temp);
	}

	return temp;
}

AVLNode* AVLTree::RotateLL(AVLNode* parent) {
	AVLNode* pNode = parent;
	AVLNode* cNode = pNode->left;

	pNode->left = cNode->right;
	cNode->right = pNode;

	return cNode;
}

AVLNode* AVLTree::RotateRR(AVLNode* parent) {
	AVLNode* pNode = parent;
	AVLNode* cNode = pNode->right;

	pNode->right = cNode->left;
	cNode->left = pNode;

	return cNode;
}

AVLNode* AVLTree::RotateLR(AVLNode* parent) {
	parent->left = RotateRR(parent->left);

	return RotateLL(parent);
}

AVLNode* AVLTree::RotateRL(AVLNode* parent) {
	parent->right = RotateLL(parent->right);

	return RotateRR(parent);
}