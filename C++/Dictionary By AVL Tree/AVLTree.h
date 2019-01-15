#pragma once

#include "AVLNode.h"
#include <algorithm>

using std::string;

class AVLTree {
public:
	AVLNode* Insert(AVLNode*, const string&, const string&);
	AVLNode* Search(AVLNode*, const string&);
	AVLNode* Delete(AVLNode*&, const string&);
	void Inorder(AVLNode*);

private:
	int Height(AVLNode*);
	int Diff(AVLNode*);

	AVLNode* Balance(AVLNode*);

	AVLNode* RotateLL(AVLNode*);
	AVLNode* RotateRR(AVLNode*);
	AVLNode* RotateLR(AVLNode*);
	AVLNode* RotateRL(AVLNode*);
};