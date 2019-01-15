#include "phoneManager.h"

phoneManager* ManagerCtor() {
	phoneManager* instance = (phoneManager*)malloc(sizeof(phoneManager));
	memset(instance->phones, 0, LIST_NUM * sizeof(phoneData*));
	instance->numOfPhones = 0;

	return instance;
}

void ManagerDtor() {
#pragma omp parallel for
	for (int i = 0; manager->phones[i] != NULL; i++)
		PhoneDtor(manager->phones[i]);

	SAFE_FREE(manager);
}

void InputPhoneData() {
	char* name = Input("이름 입력: ");
	char* phoneNum = Input("전화번호 입력: ");

	int* nameDatas = GetSamePhoneIndex(name, NAME);

	if (nameDatas[0] != -1) {
		int* phoneNumDatas = GetSamePhoneIndex(phoneNum, PHONENUM);

		if (phoneNumDatas[0] != -1) {
			puts("동일한 정보가 있습니다.");
			SAFE_FREE(nameDatas);
			SAFE_FREE(phoneNumDatas);
			return;
		}
	}

	manager->phones[manager->numOfPhones] = PhoneCtor(name, phoneNum);
	manager->numOfPhones++;

	SAFE_FREE(name);
	SAFE_FREE(phoneNum);
	SAFE_FREE(nameDatas);
	Save();
	puts("입력이 완료되었습니다.");
}

void ShowAllData() {
#pragma omp parallel for
	for (int i = 0; i < manager->numOfPhones; i++)
		ShowPhoneInfo(manager->phones[i]);
}

void SearchPhoneData() {
	char* wantName = Input("찾는 이름은?: ");
	int* nameDatas = GetSamePhoneIndex(wantName, NAME);
	SAFE_FREE(wantName);

	if (nameDatas[0] != -1) {
#pragma omp parallel for
		for (int i = 0; nameDatas[i] != -1; i++)
			ShowPhoneInfo(manager->phones[nameDatas[i]]);
	}

	else puts("입력하신 이름이 없습니다");
}

void DeletePhoneData() {
	char* deleteName = Input("지우려는 이름은?: ");
	int* nameDatas = GetSamePhoneIndex(deleteName, NAME);
	SAFE_FREE(deleteName);

	if (nameDatas[0] != -1) {
		int choice, i = 0;

		while (nameDatas[i++] != -1) {
			printf("NUM. %d\n", i);
			ShowPhoneInfo(manager->phones[nameDatas[i - 1]]);
		}

		printf("선택: ");
		scanf("%d", &choice);

		if (!choice) {
			puts("취소 되었습니다.");
			return;
		}

#pragma omp parallel for
		for (int delPhone = nameDatas[choice - 1]; delPhone < manager->numOfPhones - 1; delPhone++)
			manager->phones[delPhone] = manager->phones[delPhone + 1];

		manager->numOfPhones--;
		Save();
		puts("삭제가 완료되었습니다.");
	}

	else puts("입력하신 이름이 없습니다");
}

void ChangePhoneNum() {
	char* changeName = Input("바꾸려는 전화번호의 이름은?: ");
	int* nameDatas = GetSamePhoneIndex(changeName, NAME);

	if (nameDatas[0] != -1) {
		int choice, i = 0;

		while (nameDatas[i++] != -1) {
			printf("NUM. %d\n", i);
			ShowPhoneInfo(manager->phones[nameDatas[i - 1]]);
		}

		printf("선택: ");
		scanf("%d", &choice);

		system("cls");

		ShowPhoneInfo(manager->phones[nameDatas[choice - 1]]);
		
		InputFlush();
		char* changeNum = Input("바꿀 전화번호: ");
		strcpy(manager->phones[nameDatas[choice - 1]]->phoneNum, changeNum);
		SAFE_FREE(changeNum);

		Save();
		puts("변경 되었습니다");
	}
	else puts("입력하신 이름이 없습니다.");
}

void InitAll() {
	InitSave();
	manager->numOfPhones = 0;
	puts("초기화 되었습니다");
}

int* GetSamePhoneIndex(char* target, int mode) {
	int(*Check[2])(phoneData*, char*) = { CheckName, CheckPhoneNum };

	int* result = (int*)malloc((manager->numOfPhones + 1) * sizeof(int));
	int numOfResult = 0;
	memset(result, -1, (manager->numOfPhones + 1) * sizeof(int));

#pragma omp parallel for
	for (int i = 0; i < manager->numOfPhones; i++) {
		if (Check[mode](manager->phones[i], target))
			result[numOfResult++] = i;
	}

	return result;
}