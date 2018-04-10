#include "star.h"

int main() {
	int film, mode;
	void(*fp[MAX_MODE])(int) = { square, triangle_1, triangle_2, triangle_3, triangle_4, pyramid, reverse, diamond, hourglass };

	do {
		film = input("찍을 개수를 입력해주세요. ");
	} while (film > 20);

	do {
		mode = input("사각형은 1, 삼각형 1은 2, 삼각형 2는 3,\n삼각형 3은 4, 삼격형 4는 5, 피라미드는 6,\n역피라미드는 7, 다이아몬드는 8, 모래시계는 9를 입력해주세요. ");
	} while (!(0 < mode && mode <= MAX_MODE));

	fp[mode - 1](film);

	system("pause");
	return 0;
}
