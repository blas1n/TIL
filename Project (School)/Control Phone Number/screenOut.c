#include "common.h"
#include "phoneData.h"
#include "screenOut.h"

void ShowMenu() {
	system("cls");

	printf("--------메뉴------------\n");
	printf(" 1. 전화번호 입력\n");
	printf(" 2. 전체 정보 출력\n");
	printf(" 3. 전화번호 검색\n");
	printf(" 4. 전화번호 삭제\n");
	printf(" 5. 저장 목록 초기화\n");
	printf(" 6. 종료\n");
	printf("------------------------\n");
	printf("선택> ");
}

void ShowPhoneInfo(phoneData* phone) {
	printf("┌----------------------\n");
	printf("| ▶ 이름: %s\n", phone->name);
	printf("| ▶ 전화번호: %s\n", phone->phoneNum);
	printf("└-----------------------\n\n");
}