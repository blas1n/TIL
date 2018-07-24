#include "common.h"
#include "phoneData.h"
#include "screenOut.h"
#include "phoneFunc.h"

#define LIST_NUM 100

int numOfData = 0;
phoneData* phoneList[LIST_NUM];

void InputPhoneData() {
	phoneList[numOfData] = (phoneData*)malloc(sizeof(phoneData));

	printf("이름 입력: ");
	scanf("%s", phoneList[numOfData]->name);
	printf("전화번호 입력: ");
	scanf("%s", phoneList[numOfData]->phoneNum);
	printf("입력이 완료되었습니다.\n");
	numOfData++;
}

void ShowAllData() {
	for (int i = 0; i < numOfData; i++)
		ShowPhoneInfo(phoneList[i]);
}

void SearchPhoneData() {
	char* wantName = InputName();
	
	int numOfPhone = Search(wantName);

	if (numOfPhone != -1)
		ShowPhoneInfo(phoneList[numOfPhone]);
	else
		printf("입력하신 이름이 없습니다\n");

	free(wantName);
}

void DeletePhoneData() {
	char* deleteName = InputName();

	int numOfPhone = Search(deleteName);

	if (numOfPhone != -1) {
 		for (int i = numOfPhone; i < numOfData; i++)
			phoneList[i] = phoneList[i + 1];
		numOfData--;
		printf("삭제가 완료되었습니다.\n");
	}

	else
		printf("입력하신 이름이 없습니다\n");

	free(deleteName);
}

int Search(char* name) {
	for (int i = 0; i < numOfData; i++) {
		if (!strcmp(name, phoneList[i]->name))
			return i;
	}

	return -1;
}

char* InputName() {
	char* name = (char*)malloc(NAME_LEN * sizeof(char));
	printf("찾는 이름은? ");
	scanf("%s", name);

	return name;
}

void Destructor() {
	for (int i = 0; phoneList[i] != NULL; i++)
		free(phoneList[i]);
}