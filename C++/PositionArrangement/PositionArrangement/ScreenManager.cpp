#include "ScreenManager.h"
#include <iostream>
#include <iomanip>
#include <string>

using std::string;

void ScreenManager::PrintPos(
	std::array<string, PositionManager::MAX_STUDENT> enumPos) {

	string pos[5][6];
	std::fill(&pos[0][0], &pos[4][5], "");

	pos[0][1] = "±³Å¹";

	for (int i = 0; i < PositionManager::MAX_STUDENT; i++) {
		const auto& posIndex = POS_MAPPER[i];
		pos[posIndex.first][posIndex.second] = enumPos[i];
	}

	for (const auto& row : pos) {
		for (const auto& name : row) {
			std::cout << std::setw(10) << name;
		}

		std::cout << std::endl;
	}
}