#include <stdio.h>

int calc(int sNum, int dNum, unsigned char oper);

int main() {
	int num[2] = {0, };
	int value = 0;
	unsigned char oper = 0;
	
	printf("수식을 입력하세요 : ");
	scanf("%d %c %d", &num[0], &oper, &num[1]);
	
	while(1) {
		value = calc(num[0], num[1], oper);
		system("cls");
		
		printf("%d", value);
		num[1], oper = 0;
		num[0] = value;
		
		scanf(" %c %d", &oper, &num[1]);
	} 
	
	return 0;
}

int calc(int sNum, int dNum, unsigned char oper) {
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
			if(dNum == 0) {
				printf("잘못된 수식입니다.\n");
				Sleep(2000);
				break; 
			}
			value = sNum / dNum;
			break;
			
		default :
			system("cls");
			printf("계산할 수 없음");
			Sleep(1500);
			
			return sNum;
	}
		
	return value;
}
