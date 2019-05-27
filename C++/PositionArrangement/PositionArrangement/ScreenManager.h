#pragma once

#include "PositionManager.h"
#include <iostream>
#include <string>

class ScreenManager {
private:
	constexpr static std::pair<int, int> POS_MAPPER[PositionManager::MAX_STUDENT] = {
		std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, 4), std::make_pair(1, 5),
		std::make_pair(2, 0), std::make_pair(2, 1), std::make_pair(2, 4), std::make_pair(2, 5),
		std::make_pair(3, 0), std::make_pair(3, 1), std::make_pair(3, 2), std::make_pair(3, 3),
		std::make_pair(3, 4), std::make_pair(3, 5), std::make_pair(4, 0), std::make_pair(4, 1),
		std::make_pair(4, 2), std::make_pair(4, 3), std::make_pair(4, 4), std::make_pair(4, 5),
	};

public:
	static void PrintPos(std::array<std::string, PositionManager::MAX_STUDENT> enumPos);

	static inline void Print(std::string msg) {
		std::cout << msg << std::endl;
	}

	template <class InputType>
	static InputType Input(std::string msg) {
		InputType out;

		std::cout << msg;
		std::cin >> out;

		return out;
	}
};

