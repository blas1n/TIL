#include "PositionManager.h"
#include "StringField.h"
#include <vector>
#include <fstream>

using namespace StringField;

PositionManager::PositionManager()
	: students(std::map<std::string, int>()) {

	std::ifstream file(FILE_PATH);

	if (!file.is_open())
		throw FILE_ERROR;

	file.close();

	LoadPosition();
}

void PositionManager::MixPosition() {
	std::vector<int> possiblePos(std::begin(INIT_NUMS), std::end(INIT_NUMS));
	
	for (auto& student : students) {
		int pos = rand() % possiblePos.size();
		student.second = possiblePos[pos];
		possiblePos.erase(possiblePos.begin() + pos);
	}

	SavePosition();
}

void PositionManager::AddStudent(const std::string& name) {
	if (students.size() >= MAX_STUDENT) return;

	int num = 0;

	for (const auto& student : students) {
		if (student.second != ++num)
			break;
	}

	students.insert(std::make_pair(name, num));

	SavePosition();
}

void PositionManager::DelStudent(const std::string& name) {
	if (students.empty()) return;

	students.erase(name);

	SavePosition();
}

void PositionManager::SavePosition() {
	std::ofstream file(FILE_PATH);

	for (const auto& student : students)
		file << student.first.c_str() << " " << student.second << "\n";

	file.close();
}

void PositionManager::LoadPosition() {
	students.clear();
	
	std::ifstream file(FILE_PATH);
	
	char name[10];
	int pos;

	while (file >> name >> pos)
		students.insert(std::make_pair(name, pos));

	file.close();
}

std::array<std::string, PositionManager::MAX_STUDENT>
PositionManager::EnumPosition() const {
	std::array<std::string, MAX_STUDENT> out;
	std::fill(out.begin(), out.end(), NULL_POS);

	for (const auto& student : students)
		out[student.second - 1] = student.first;

	return out;
}