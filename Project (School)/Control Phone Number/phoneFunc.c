#include "common.h"
#include "phoneData.h"
#include "screenOut.h"
#include "phoneFunc.h"

#define LIST_NUM 100

int numOfData = 0;
phoneData* phoneList[LIST_NUM];

void Constructor() {
	FILE* saveFile = fopen("PhoneData.txt", "r");
	if (saveFile == NULL) return;

	char str[NAME_LEN];

	while (fscanf(saveFile, "%s", str) != EOF) {
		phoneList[numOfData] = (phoneData*)malloc(sizeof(phoneData));

		strcpy(phoneList[numOfData]->name, str);
		fscanf(saveFile, "%s", phoneList[numOfData]->phoneNum);
		numOfData++;
	}

	fclose(saveFile);
}

void InputPhoneData() {
	phoneList[numOfData] = (phoneData*)malloc(sizeof(phoneData));

	printf("이름 입력: ");
	scanf("%s", phoneList[numOfData]->name);
	printf("전화번호 입력: ");
	scanf("%s", phoneList[numOfData]->phoneNum);

	int* phoneDatas = Search(phoneList[numOfData]->name);

	if (phoneDatas[0] != -1) {
		for (int i = 0; phoneDatas[i] != -1; i++) {
			if (!strcmp(phoneList[numOfData]->phoneNum, phoneList[phoneDatas[i]]->phoneNum)) {
				printf("동일한 정보가 있습니다.\n");
				SAFE_FREE(phoneDatas);
				return;
			}
		}
	}

	SAFE_FREE(phoneDatas);
	printf("입력이 완료되었습니다.\n");
	numOfData++;
}

void ShowAllData() {
	for (int i = 0; i < numOfData; i++)
		ShowPhoneInfo(phoneList[i]);
}

void SearchPhoneData() {
	char* wantName = InputName();
	int* phoneDatas = Search(wantName);

	if (phoneDatas[0] != -1) {
		for (int i = 0; phoneDatas[i] != -1; i++)
			ShowPhoneInfo(phoneList[phoneDatas[i]]);
	}
		
	else printf("입력하신 이름이 없습니다\n");

	SAFE_FREE(wantName);
}

void DeletePhoneData() {
	char* deleteName = InputName();
	int* phoneDatas = Search(deleteName);

	if (phoneDatas[0] != -1) {
		int choice, i = 0;

		while (phoneDatas[i++] != -1) {
			printf("NUM. %d\n", i);
			ShowPhoneInfo(phoneList[phoneDatas[i - 1]]);
		}

		printf("선택: ");
		scanf("%d", &choice);

 		for (int delPhone = phoneDatas[choice - 1]; delPhone < numOfData - 1; delPhone++)
			phoneList[delPhone] = phoneList[delPhone + 1];

		numOfData--;
		printf("삭제가 완료되었습니다.\n");
	}

	else
		printf("입력하신 이름이 없습니다\n");

	SAFE_FREE(deleteName);
}

void InitFile() {
	remove("PhoneData.txt");
	numOfData = 0;
	printf("초기화 되었습니다\n");
}

int* Search(char* name) {
	int* result = (int*)malloc((numOfData + 1) * sizeof(int));
	int numOfResult = 0;
	memset(result, -1, (numOfData + 1) * sizeof(int));

	for (int i = 0; i < numOfData; i++) {
		if (!strcmp(name, phoneList[i]->name))
			result[numOfResult++] = i;
	}

	return result;
}

char* InputName() {
	char* name = (char*)malloc(NAME_LEN * sizeof(char));
	printf("찾는 이름은? ");
	scanf("%s", name);
	return name;
}

void Destructor() {
	FILE* saveFile = fopen("PhoneData.txt", "w");

	for (int i = 0; i < numOfData; i++) {
		fprintf(saveFile, "%s\n", phoneList[i]->name);
		fprintf(saveFile, "%s\n", phoneList[i]->phoneNum);
	}

	fclose(saveFile);

	for (int i = 0; phoneList[i] != NULL; i++) { SAFE_FREE(phoneList[i]); }
}