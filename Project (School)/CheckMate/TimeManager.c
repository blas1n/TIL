#include "TimeManager.h"

Time GetTakeTime() {
	return takeTime;
}

void WINAPI IncreaseTime() {
	takeTime.second++;

	if (takeTime.second > 59) {
		takeTime.second = 0;

		if (++takeTime.minute > 59)
			takeTime.minute = 0;
	}
}