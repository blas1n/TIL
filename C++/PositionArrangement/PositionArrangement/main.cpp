#include <string>
#include "PositionManager.h"
#include "ScreenManager.h"
#include "StringField.h"

using namespace StringField;

int main() {
	try {
		PositionManager posManager;
		std::string buffer;
		int choice = 0;

		ScreenManager::Print(INTRO);

		while (true) {
			choice = ScreenManager::Input<int>(SELECT);

			if (!choice) {
				ScreenManager::Print(END);
				return 0;
			}

			switch (choice) {
			case 1:
				ScreenManager::Print(START_ARRANGE);
				posManager.MixPosition();
				ScreenManager::Print(END_ARRANGE);

			case 2:
				ScreenManager::PrintPos(posManager.EnumPosition());
				break;

			case 3:
				for (auto&& student : posManager.EnumPosition())
					ScreenManager::Print(std::forward<std::string>(student));

				break;

			case 4:
				buffer = ScreenManager::Input<std::string>(ADD_STUDENT);
				posManager.AddStudent(buffer);
				break;

			case 5:
				buffer = ScreenManager::Input<std::string>(DEL_STUDENT);
				posManager.DelStudent(buffer);
				break;
			}
		}
	}

	catch (std::string msg) {
		ScreenManager::Print(msg);
		return 0;
	}

	return 1;
}