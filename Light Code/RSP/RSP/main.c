#include "RSP.h"

int main() {
	int UI, CI;
	int* pUI = &UI;
	int* pCI = &CI;
	UI = Input("값을 입력하슈: ", pUI);
	CI = random;
	Process(UI, CI);

	system("pause");
	return 0;
}
