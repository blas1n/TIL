#pragma once

#include <string>

using std::string;

struct AVLNode {
	string key;
	string value;

	AVLNode* left;
	AVLNode* right;

public:
	AVLNode();
	~AVLNode();
};

std::ostream& operator<<(std::ostream& os, AVLNode* node);