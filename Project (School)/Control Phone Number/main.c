#include "common.h"
#include "screenOut.h"
#include "phoneFunc.h"

enum {Input = 1, SHOWALL, SEARCH, DELETE, QUIT};

int main() {
	int inputMenu = 0;

	while (1) {
		ShowMenu();
		fputs("선택하세요 : ", stdout);
		scanf("%d", &inputMenu);
		fflush(stdin);

		switch (inputMenu) {
		case Input:
			InputPhoneData();
			break;

		case SHOWALL:
			ShowAllData();
			break;

		case SEARCH:
			SearchPhoneData();
			break;

		case DELETE:
			DeletePhoneData();
			break;

		case QUIT: {
			puts("이용해 주셔서 감사합니다.");
			return 0;
		}
		}

		system("pause");
	}

	Destructor();
	return 0;
}