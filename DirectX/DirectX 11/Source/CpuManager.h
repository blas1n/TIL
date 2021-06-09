#pragma once

#include <pdh.h>

class CpuManager final
{
public:
	void Initialize();
	void Frame();
	void Release() noexcept;

	int GetCpuPercentage() const noexcept { return canReadCpu ? cpuUsage : 0; }

private:
	HCOUNTER counterHandle;
	HQUERY queryHandle;

	unsigned long long lastSampleTime;
	int cpuUsage;

	bool canReadCpu;
};
