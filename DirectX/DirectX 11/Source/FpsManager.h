#pragma once

class FpsManager final
{
public:
	void Frame();

	int GetFps() const noexcept { return fps; }

private:
	int fps, count;
	unsigned long startTime;
};
