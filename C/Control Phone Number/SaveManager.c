#include "SaveManager.h"

void Save() {
	FILE* numOfDataFile = fopen(NUMOFDATA_FILE, "wb");
	fwrite(&(manager->numOfPhones), sizeof(int), 1, numOfDataFile);
	fclose(numOfDataFile);

	FILE* saveFile = fopen(PHONE_FILE, "wb");
	for (int i = 0; i < manager->numOfPhones; i++)
		fwrite(manager->phones[i], sizeof(phoneData), 1, saveFile);
	fclose(saveFile);
}

void Load() {
	FILE* numOfDataFile = fopen(NUMOFDATA_FILE, "rb");
	if (numOfDataFile == NULL) return;
	fread(&(manager->numOfPhones), sizeof(int), 1, numOfDataFile);
	fclose(numOfDataFile);

	for (int i = 0; i < manager->numOfPhones; i++)
		manager->phones[i] = (phoneData*)malloc(sizeof(phoneData));

	FILE* saveFile = fopen(PHONE_FILE, "rb");
	if (saveFile == NULL) return;
	for (int i = 0; i < manager->numOfPhones; i++)
		fread(manager->phones[i], sizeof(phoneData), 1, saveFile);
	fclose(saveFile);
}

int InitSave() {
	if (remove(PHONE_FILE)) return 1;
	else return 0;
}