#include "System.h"

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	System system;
	int result = 0;
	if (system.Init())
		result = system.Run();
	
	system.Release();
	return result;
}