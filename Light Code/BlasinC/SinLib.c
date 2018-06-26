#include "SinLib.h"

void* Input(cstr guide, const int maxCount) {
	char* input = (char*)calloc(maxCount, sizeof(char));

	Print(guide);
	Scan(input, maxCount, stdin);

	SAFE_FREE(input);
	return input;
}

void Print(cstr guide) {
	fputs(guide, stdout);
}

void Println(cstr guide) {
	fputs(guide + '\n', stdout);
}

char GetKey() {
	if (_kbhit())
		return _getch();
}

void Scan(str buf, int maxCount, FILE* fp) {
	int i;

	for (i = 0; i < maxCount - 1; i++) {
		buf[i] = fgetc(fp);

		if (buf[i] == '\n') {
			InputFlush();
			for (; i < maxCount; i++) { buf[i] = '\0'; }
			return;
		}
	}

	InputFlush();
	buf[maxCount - 1] = '\0';
}

void InputFlush() {
	for (; fgetc(stdin) != EOF;);
}

int Random(int area, int start) {
	return (rand() % area) + start;
}

void gotoxy(int posX, int posY) {
	COORD pos = { posX, posY };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Delay(float cool) {
	Sleep(cool / 1000);
}