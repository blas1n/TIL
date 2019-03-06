#include "SystemClass.h"

int APIENTRY _tWinMain(HINSTANCE hIn, HINSTANCE prev, PTSTR cmd, int cShow) {
	return SystemClass::GetInstance().Run();
}