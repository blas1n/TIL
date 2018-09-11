#include <stdio.h>

/* [Notice for C/C++]
* - 기본 제공되는 뼈대 코드는 입출력의 이해를 돕기위해 제공되었습니다.
* - 뼈대코드의 활용은 선택사항이며 코드를 직접 작성하여도 무관합니다.
*
* - 별도의 병렬 처리나 시스템콜, 네트워크/파일접근 등을 하지 마세요
* - main은 int형으로 선언한 후 return 0를 항상 하는 것을 권유합니다
* - 지역변수 배열 크기에 의한 Stack Overflow 에러에 주의하세요
* - (C++) cin/cout보다 scanf/printf를 사용하기를 권장합니다. (입출력 성능에 의한 이유)
*/

int main()
{   //프로그램의 시작부 
	int player[5];
	int winner = 0; //이 변수에 승리하는 사람의 수를 저장한다
	int i, j, allSame, count = 1;
	for (i = 0; i < 5; i++) {
		scanf("%d", &player[i]);
	}

	allSame = player[0];
	for (i = 0; i < 5; i++) {

		if (player[i] != allSame) {
			allSame = player[i];
			count++;

			if (count > 2) {
				printf("0\n");
				return 0;
			}
		}
	}

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (i == j) { break; }

			if (player[i] > player[j]) {
				winner++;
				break;
			}

			if (player[i] == 1 && player[j] == 3) {
				winner++;
				break;
			}
		}
	}

	//답을 출력한다 
	printf("%d\n", winner);
	return 0;
}