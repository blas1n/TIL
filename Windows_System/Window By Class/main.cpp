#include "MyApp.h"
#include <tchar.h>

int WINAPI _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	MyApp* winApp = new MyApp(hIn);
	winApp->Run();

	delete winApp;

	return 0;
}