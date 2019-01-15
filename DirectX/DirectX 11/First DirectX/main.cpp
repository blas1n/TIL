#include "SystemClass.h"

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	SystemClass* System;
	bool result;

	System = new SystemClass;
	if (!System) return 1;

	result = System->Init();
	if (result) System->Run();

	System->ShutDown();
	delete System;
	System = nullptr;

	return 0;
}