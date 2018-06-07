#include <stdio.h>

char getKey();

int main() {
	int time[3] = {0, 0, 0};
	char key = 0; 
	
	for(;;) {
		key = 0;
		time[2]++;
		
		if(time[2] >= 60) {
			time[2] == 0;
			time[1]++;
		}
		
		if(time[1] >= 60) {
			time[1] == 0;
			time[0]++;
		}

		printf("%d : %d : %d\n일시 정지는 1, 종료는 0을 누르세요.\n\n", time[0], time[1], time[2]);

		key = getKey();
		
		if(key == '1') {
			system("pause");
		}
		
		if(key == '0') {
			return 0;
		}
		
		Sleep(1000);
		system("cls");
	}
	
	return 0;
}

char getKey() {
	if(kbhit()) {
        return getch();
    }
    
    return 0;
}
