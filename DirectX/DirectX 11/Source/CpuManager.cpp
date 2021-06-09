#include "CpuManager.h"

#pragma comment(lib, "pdh.lib")

void CpuManager::Initialize()
{
	PDH_STATUS status = PdhOpenQuery(nullptr, 0, &queryHandle);
	canReadCpu = status == ERROR_SUCCESS;
	
	status = PdhAddCounter(queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &counterHandle);
	canReadCpu = canReadCpu && status == ERROR_SUCCESS;

	lastSampleTime = GetTickCount64();
	cpuUsage = 0;
}

void CpuManager::Frame()
{
	if (!canReadCpu || (lastSampleTime + 1000) >= GetTickCount64())
		return;

	lastSampleTime = GetTickCount64();
	PdhCollectQueryData(queryHandle);

	PDH_FMT_COUNTERVALUE value;
	PdhGetFormattedCounterValue(counterHandle, PDH_FMT_LONG, nullptr, &value);
	cpuUsage = static_cast<int>(value.longValue);
}

void CpuManager::Release() noexcept
{
	if (canReadCpu)
		PdhCloseQuery(queryHandle);
}
