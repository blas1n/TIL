#pragma once

class TimerManager final
{
public:
	bool Initialize();
	void Frame();

	float GetTime() const noexcept { return frameTime; }

private:
	float frameTime, ticksPerMs;
	long long frequency, startTime;
};
