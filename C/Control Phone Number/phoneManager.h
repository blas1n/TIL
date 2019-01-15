#ifndef __PHONEMANAGER_H__
#define __PHONEMANAGER_H__

#include "common.h"
#include "screenOut.h"
#include "phoneData.h"
#include "SaveManager.h"

#define LIST_NUM 100

enum {
	NAME, PHONENUM
};

typedef struct PhoneManager {
	phoneData* phones[LIST_NUM];
	int numOfPhones;
} phoneManager;

phoneManager* ManagerCtor();
void ManagerDtor();
void InputPhoneData();
void ShowAllData();
void SearchPhoneData();
void DeletePhoneData();
void ChangePhoneNum();
void InitAll();
int* GetSamePhoneIndex(char* target, int mode);

phoneManager* manager;

#endif