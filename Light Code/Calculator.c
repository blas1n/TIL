#include<stdio.h>
 
int main(void){
	float num[2] = {0, 0};
	float value = 0;
	unsigned char a = 0;

 	for(;;) {
 		system("cls");
 		printf("수식을 입력하시오(종료하려면 000을 입력해주세요) : ");
		scanf("%f %c %f", &num[0], &a, &num[1]);
		system("cls");
 
		switch(a){
		case '+':
			value = num[0] + num[1];
			printf( "%.1f %c %.1f = %.1f", num[0], a, num[1], value);
			Sleep(2000);
			break;
			
		case '-':
			value = num[0] - num[1];
			printf( "%.1f %c %.1f = %.1f", num[0], a, num[1], value );
			Sleep(2000);
			break;
			
		case '*':
			value = num[0] * num[1];
			printf( "%.1f %c %.1f = %.1f", num[0], a, num[1], value );
			Sleep(2000);
			break;
			
		case '/': 
			if(num[1] == 0) {
				printf("잘못된 수식입니다.\n");
				Sleep(2000);
				break; 
			}
			value = (int)num[0] / (int)num[1]; 
			printf( "%d %c %d = %.1f", num[0], a, num[1], value);
			Sleep(2000);
			break;
		
		case '0':
			printf("감사합니다\n");	
			return 0;
		default:
			printf( "계산할 수 없음" );
			Sleep(2000);
			break;
		}
	}
	return 0;
}
