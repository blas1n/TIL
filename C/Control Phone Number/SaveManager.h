#ifndef __SAVEMANAGER_H__
#define __SAVEMANAGER_H__

#include "common.h"
#include "phoneManager.h"

#define PHONE_FILE "PhoneData.dat"
#define NUMOFDATA_FILE "NumOfData.dat"

void Save();
void Load();
int InitSave();

#endif