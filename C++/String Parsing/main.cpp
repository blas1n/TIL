#include <iostream>
#include <string>
#include <vector>
#include <regex>

#define MAX_INPUT 42
 
using namespace std;

// 띄어쓰기를 기준으로 자른 문자열 하나 하나
class String : string {
private:
	string text;
public:
	// 입력받은 값을 text에 할당
	String(string input);

	// 자신이 key를 가지고 있는지 확인
	bool HaveKey(const char key) const;

	// yes -> 1, no -> 0으로 변환
	void BoolToInt();

	// 16진수 문자열 값을 10진수 문자열 값으로 변경
	void HexToInt();

	string Get();

	void operator+=(string userInput);

	friend ostream& operator<< (ostream& os, String& message);
};

// String 전체를 관리하는 클래스
class Parser : vector<String> {
private:
	vector<String> message;
private:
	// key 값을 가지고 있는 String의 vector를 가져옴
	vector<int> Search(const string key = " ");

	void Swap(int pos1, int pos2);

	// 16 진수의 위치를 찾아서 10진수로 변환
	void TidyHex();

	// yes / no의 위치를 찾아서 1 / 0으로 변환, 뒤와 순서를 바꿈
	void TidyBool();

	// 필요한 부분 (숫자인 부분)만 추출
	void Extraction();

	// 해상도를 NxN, N, N 양식으로 묶음
	void TieValue();
public:
	// 위의 함수 4개를 묶은 캡슐 메서드
	void Parsing();

	// 벡터의 뒤에 새로운 문자열을 추가하는 연산자
	void operator+= (string userInput);

	// message를 반환
	vector<String> Get();

	// message를 메모리 해제
	~Parser();
};

int main() {
	Parser parser;
	string input;

	// 입력받는 부분
	for (int i = MAX_INPUT; i > 0; i--) {
		cin >> input;
		parser += input;
	}

	// 파싱하는 부분
	parser.Parsing();

	// 출력하는 부분
	for (auto result : parser.Get())
		cout << result;

	system("pause");
	return 0;
}

String::String(string input)
	: text(input)
{}

bool String::HaveKey(const char key) const {
	if (text.find(key) != string::npos)
		return true;
	return false;
}

void String::BoolToInt() {
	if (text == "yes")
		text = "1";

	if (text == "no")
		text = "0";
}

void String::HexToInt() {
	text = to_string(strtoul(text.c_str(), NULL, 16));
}

string String::Get() {
	return text;
}

void String::operator+= (string userInput) {
	text += userInput;
}

ostream& operator<< (ostream& os, String& message) {
	cout << message.text << endl;
	return os;
}

vector<int> Parser::Search(const string key) {
	vector<int> result;
	int i = 0;

	for (auto nowString : message) {
		if (nowString.Get().find(key) != string::npos) {
			result.push_back(i);
		}
		i++;
	}

	return result;
}

void Parser::Swap(int pos1, int pos2) {
	String temp = message[pos1];
	message[pos1] = message[pos2];
	message[pos2] = temp;
}

void Parser::TidyHex() {
	vector<int> hexVal = (Search("0x"));

	for (auto hexText : hexVal)
		message[hexText].HexToInt();
}

void Parser::TidyBool() {
	vector<int> boolVal = Search("yes");

	for (auto boolText : boolVal) {
		message[boolText].BoolToInt();
		Swap(boolText, boolText + 1);
	}

	boolVal = Search("no");

	for (auto boolText : boolVal) {
		message[boolText].BoolToInt();
		Swap(boolText, boolText + 1);
	}
}

void Parser::Extraction() {
	regex number("[0-9]+");
	
	vector<String>::iterator agency = message.begin();

	while(agency != message.end()) {
		if (!(regex_match(agency->Get(), number)))
			agency = message.erase(agency);
		else
			agency++;
	}
}

void Parser::TieValue() {
	for (int index = 4; index < 21; index += 4) {
		message[index] += "x" + message[index + 1].Get() + ", "
			+ message[index + 2].Get() + ", " + message[index + 3].Get();
	}

	vector<String>::iterator agency = message.begin() + 5;

	while (agency <= message.end() - 2) {
		agency = message.erase(agency, agency + 3);
		if (agency != message.end())
			agency++;
	}
}

vector<String> Parser::Get() {
	return message;
}

void Parser::Parsing() {
	TidyHex();
	TidyBool();
	Extraction();
	TieValue();
}

void Parser::operator+= (string userInput) {
	message.push_back(userInput);
}

Parser::~Parser() {
	vector<String>().swap(message);
}