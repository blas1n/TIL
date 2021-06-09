#include "FpsManager.h"
#include <Windows.h>

#pragma comment(lib, "winmm.lib")

void FpsManager::Frame()
{
	++count;

	if (timeGetTime() >= startTime + 1000)
	{
		fps = count;
		count = 0;

		startTime = timeGetTime();
	}
}
