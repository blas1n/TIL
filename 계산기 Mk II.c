#include <stdio.h>
#include <conio.h>

int calc(int sNum, int dNum, char oper);

int main() {
	int num[2] = {0, 0};
	int value = 0;
	int option = 0;
	char oper = 0;
	
	while(1) {
		num[0] = 0;
		num[1] = 0;
		value = 0;
		oper = 0;
		option = 0;
		
		printf("수식을 입력해주세요 : ");
		scanf("%d %c %d", &num[0], &oper, &num[1]);
		
		while(1) {
			value = calc(num[0], num[1], oper);
			system("cls");
			
			printf("%d", value);
			num[1] = 0;
			oper = 0;
			num[0] = value;
			
			scanf(" %c %d", &oper, &num[1]);
		}
	}
	
	return 0;	
}

int calc(int sNum, int dNum, char oper) {
	int value = 0;
	
	switch(oper) {
		case '+' :
			value = sNum + dNum;
			break;
		
		case '-' :
			value = sNum - dNum;
			break;
		
		case '*' :
			value = sNum * dNum;
			break;
		
		case '/' :
			value = sNum / dNum;
			break;
			
		default :
			system("cls");
			printf("계산할 수 없음");
			Sleep(2000);
			
			return sNum;
	}
	
	return value;
}
