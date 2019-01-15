#include "AVLTree.h"
#include <iostream>

using std::cout;
using std::endl;

bool Choice(int& choice);
void Input(string* key, string* value);

int main() {
	AVLTree* avl = new AVLTree();
	AVLNode* root = nullptr;
	AVLNode* result = nullptr;

	string inputKey, inputValue, resultString;
	int choice;

	cout << "치고 싶은 거 다쳐도 되는 백과사전\n" << endl;

	while (Choice(choice)) {
		switch (choice) {
		case 1:
			Input(&inputKey, nullptr);
			result = avl->Search(root, inputKey);
			cout << result << endl;
			break;

		case 2:
			Input(&inputKey, &inputValue);
			root = avl->Insert(root, inputKey, inputValue);
			break;

		case 3:
			Input(&inputKey, nullptr);

			result = avl->Delete(root, inputKey);
			cout << "삭제한 단어: " << result << endl;

			break;

		case 4:
			avl->Inorder(root);
			break;
		}

		cout << endl << endl;
	}

	if (result) {
		delete result;
		result = nullptr;
	}

	delete avl;
	avl = nullptr;

	return 0;
}

bool Choice(int& choice) {
	cout << "1. 단어 검색\n";
	cout << "2. 단어 추가\n";
	cout << "3. 단어 삭제\n";
	cout << "4. 전체 출력\n";
	cout << "0. 종료\n" << endl;

	cout << "선택. ";
	std::cin >> choice;
	cout << endl;

	return choice != 0;
}

void Input(string* key, string* value) {
	if (key) {
		cout << "단어를 입력하세요. ";
		std::cin >> (*key);
	}

	if (value) {
		cout << "뜻을 입력하세요. ";
		std::cin >> (*value);
	}
}