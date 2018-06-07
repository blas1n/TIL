#include <stdio.h>

int main() {
	int price, dis, reduce = 0;
	
	printf("물품의 가격을 입력해주세요 : ");
	scanf("%d", &price);
	printf("할인율을 입력해주세요(숫자만 입력해주세요) : ");
	scanf("%d", &dis);
	
	reduce = price - price * dis / 100;
	
	printf("할인 가격 : %d", reduce); 
}
