#ifndef __PHONEDATA_H__
#define __PHONEDATA_H__

#include "common.h"

typedef struct _phoneData {
	char name[30];
	char phoneNum[30];
} phoneData;

phoneData* PhoneCtor(char*, char*);
void PhoneDtor(phoneData*);
int CheckName(phoneData*, char*);
int CheckPhoneNum(phoneData*, char*);

#endif