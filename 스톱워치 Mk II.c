#include <stdio.h>
#include <stdlib.h>

int stopWatch();
int timer();
char getKey();
void close();

int main() {
	int func = 0;
	int scanReturn = 0;
	int retry = 0;
	
	while(1) {
		system("cls");
			do {
				printf("스톱워치는 1, 타이머는 2, 종료는 0을 눌러주세요. ");
				fflush(stdin);
				retry = (scanf("%d", &func) != 1 ? 1 : 0);
				system("cls");
			} while(retry == 1);
			
			switch(func) {
				case 1 : 
				stopWatch();
				break;
				
				case 2 :
				timer();
				break;
				
				case 0 :
				close();
				
				default :
				printf("잘 못된 입력 값입니다.");
				Sleep(1500);
			}
	}
	return 0;
}

int stopWatch() {
	int time[3] = {0, 0, 0};
	char key = 0;
	
	while(1) {
		key = 0;
		time[2]++;
		
		if(time[2] >= 60) {
			time[2] = 0;
			time[1]++;
		}
		
		if(time[1] >= 60) {
			time[1] = 0;
			time[0]++;
		}

		printf("%2d : %2d : %2d\n일시 정지는 1, 초기화면으로 돌아가려면 2, 종료는 0을 누르세요.\n\n", time[0], time[1], time[2]);

		key = getKey();
		
		if(key == '1') {
			system("pause");
		} else if(key == '2') {
			return 0;
		} else if(key == '0') {
			close();
		}
		
		Sleep(1000);
		system("cls");
	}
}

int timer() {
	int time[3] = {0, 0, 0};
	int retry = 0;
	char key = 0;
	
	do {
		printf("시간을 입력해주세요(시 분 초로 입력해주세요). ");
		fflush(stdin);
		retry = (scanf("%d %d %d", &time[0], &time[1], &time[2]) != 3 ? 1 : 0);
		system("cls");
	} while(retry == 1);
		
	while(1) {
		key = 0;
		time[2]--;
		
		if(time[2] < 0) {
			time[2] = 59;
			time[1]--;
		}
		
		if(time[1] < 0) {
			time[1] = 59;
			time[0]--;
		}

		printf("%d : %d : %d\n일시 정지는 1, 초기화면으로 돌아가려면 2, 종료는 0을 누르세요.\n\n", time[0], time[1], time[2]);
		

		key = getKey();
		
		if(key == '1') {
			system("pause");
		} else if(key == '2') {
			return 0;
		} else if(key == '0') {
			close();
		}
		
		if(time[0] == 0 && time[1] == 0 && time[2] == 0) {
			printf("\a시간이 다 되었습니다.\n");
			system("pause");
			break;
		}
		
		Sleep(1000);
		system("cls");
	}
}

char getKey() {
	if(kbhit()) {
        return getch();
    }
    
    return 0;
}

void close() {
	system("cls");
	printf("감사합니다.");
	exit (0);
}
