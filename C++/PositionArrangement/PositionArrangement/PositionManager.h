#pragma once

#include <map>
#include <array>
#include <string>

class PositionManager {
public:
	constexpr const static int MAX_STUDENT = 20;

private:
	constexpr const static char* FILE_PATH = "Postion.txt";
	constexpr static int INIT_NUMS[MAX_STUDENT] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
	};

public:
	PositionManager();

	void MixPosition();

	void AddStudent(const std::string& name);
	void DelStudent(const std::string& name);

	std::array<std::string, MAX_STUDENT> EnumPosition() const;

private:
	void SavePosition();
	void LoadPosition();
	
private:
	std::map<std::string, int> students;
};

