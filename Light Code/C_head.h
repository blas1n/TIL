#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_FREE(value) value = NULL; free(value);
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b
#define ADD(a, b) a + b
#define SUB(a, b) a - b
#define USUB(a, b) MAX(a, b) - MIN(a, b)
#define MULTI(a, b) a * b
#define DIV(a, b) if(b != 0) {a / b;} else {0;}

char getKey();
char* input(const char* guide);
int random(int area, int start);


/*char getKey() {
	if (kbhit())
		return getch();
}

char* input(const char* guide) {
	char buf, deliver;

	do {
		printf("%s", guide);
		putchar(buf);
		scanf_s("%s", deliver, sizeof(deliver));
		system("cls");
	} while (deliver == NULL);

	return deliver;
}

int random(int area, int start) {
	srand(time(NULL));
	return (skip + rand() % area + start);
}*/
