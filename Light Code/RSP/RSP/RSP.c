#include "RSP.h"

void Input(char* guide, int* UI, int* CI) {
	printf("%s", guide);							// 안내문 출력
	scanf("%d %d", UI, CI);							// 입력, 정상 입력 시 rerty는 2
}

void Process(int user, int com) {
	if (user == com) { printf("무승부입니다.\n"); }
	else if ((user + 1 == com) || (user + 1 == 3) && (com == 0)) { printf("승리입니다!\n"); }
	else { printf("패배입니다.\n"); }
}

int random(int area, int start) {
	srand(time(NULL));
	return (rand() % area + start);
}

char getKey() {
	if (kbhit())
		return getch();
}