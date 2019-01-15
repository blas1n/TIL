#include "common.h"
#include "phoneData.h"
#include "screenOut.h"

void ShowMenu() {
	system("cls");

	puts("--------메뉴------------");
	puts(" 1. 전화번호 입력");
	puts(" 2. 전체 정보 출력");
	puts(" 3. 전화번호 검색");
	puts(" 4. 전화번호 삭제");
	puts(" 5. 전화번호 변경");
	puts(" 6. 저장 목록 초기화");
	puts(" 7. 종료");
	puts("------------------------");
	printf("선택> ");
}

void ShowPhoneInfo(phoneData* phone) {
	puts("┌----------------------");
	printf(" | ▶ 이름: %s\n", phone->name);
	printf(" | ▶ 전화번호: %s\n", phone->phoneNum);
	puts("└-----------------------\n");
}