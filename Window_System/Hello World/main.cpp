#include <Windows.h>
#include <tchar.h>

int WINAPI WinMain(HINSTANCE hin, HINSTANCE prev, PSTR CLine, int CShow) {
	// blAs1N이라는 제목을 가지고 Hello, world!를 출력하고 확인 버튼이 있는 메세지 박스
	MessageBox(NULL, _T("Hello, world!"), _T("blAs1N"), MB_OK);
	return 0;
}