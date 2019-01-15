#include "star.h"

int input(char* guide) {
	char* deliver = (char*)malloc(sizeof(char) * 3);
	int real;

	do {
		printf("%s", guide);
		fflush(stdin);
		scanf_s("%s", deliver, sizeof(deliver)-1);
		real = atoi(deliver);
		system("cls");
	} while (real <= 0);
	
	SAFE_FREE(deliver);
	return real;
}

void square(int film, char* stars, char* blank) {
	for (int turn = 0; turn < film; turn++)
		printf("%.*s\n", film, stars);
}

void triangle_1(int film, char* stars, char* blank) {
	for (int turn = 1; turn <= film; turn++)
		printf("%.*s\n", turn, stars);
}

void triangle_2(int film, char* stars, char* blank) {
	for (int turn = film; turn > 0; turn--) 
		printf("%.*s\n", turn, stars);
}

void triangle_3(int film, char* stars, char* blank) {
	for (int turn = 1; turn <= film; turn++) 
		printf("%.*s%.*s\n", film - turn, blank, turn, stars);
}

void triangle_4(int film, char* stars, char* blank) {
	for (int turn = film; turn >= 1; turn--) 
		printf("%.*s%.*s\n", film - turn, blank, turn, stars);
}

void pyramid(int film, char* stars, char* blank) {
	for (int turn = 1; turn <= film; turn += 2) 
		printf("%.*s%.*s\n", DEV(film, turn), blank, turn, stars);
}

void reverse(int film, char* stars, char* blank) {
	for (int turn = film; turn > 0; turn -= 2) 
		printf("%.*s%.*s\n", DEV(film, turn), blank, turn, stars);
}

void diamond(int film, char* stars, char* blank) {
	for (int turn = 1; turn <= film * 2; turn += 2)
		printf("%.*s%.*s\n", OPP_1(turn, film), blank, OPP_2(turn, film), stars);
}

void hourglass(int film, char* stars, char* blank) {
	for (int turn = 1; turn <= film * 2; turn += 2) 
		printf("%.*s%.*s\n", OPP_3(turn, film), blank, OPP_4(turn, film), stars);
}

char* assign(int film, char word) {
	char* ment = (char*)malloc(sizeof(char)*film);
	for (int re = 0; re <= film; re++) ment[re] = word;
	return ment;
}