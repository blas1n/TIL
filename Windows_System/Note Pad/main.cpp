#include "MyApp.h"
#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE hPrev, LPTSTR cmd, int Cshow) {
	MyApp* winApp = new MyApp(hIn);
	winApp->Run();
	delete winApp;

	return 0;
}