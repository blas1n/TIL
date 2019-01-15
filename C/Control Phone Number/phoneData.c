#include "phoneData.h"

phoneData* PhoneCtor(char* name, char* phoneNum) {
	phoneData* instance = (phoneData*)malloc(sizeof(phoneData));

	strcpy(instance->name, name);
	strcpy(instance->phoneNum, phoneNum);

	return instance;
}

void PhoneDtor(phoneData* this) {
	SAFE_FREE(this);
}

int CheckName(phoneData* this, char* name) {
	if (!strcmp(this->name, name)) return 1;
	else return 0;
}

int CheckPhoneNum(phoneData* this, char* phoneNum) {
	if (!strcmp(this->phoneNum, phoneNum)) return 1;
	else return 0;
}