#include "TimerManager.h"
#include <Windows.h>

bool TimerManager::Initialize()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	if (!frequency) return false;

	ticksPerMs = static_cast<float>(frequency) / 1000;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));
	return true;
}

void TimerManager::Frame()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float timeDiff = static_cast<float>(curTime.QuadPart - startTime);
	frameTime = timeDiff / ticksPerMs;
	startTime = curTime.QuadPart;
}
