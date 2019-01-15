#include <iostream>
#include <sstream>
#include <string>
#include <stack>

using namespace std;

//Implement this class
//Please use stack only
class Queue {
private:
	stack<int> inputSt;
	stack<int> outputSt;
private:
	void OutputReady() {
		stack<int> buffer;

		while (!(inputSt.empty())) {
			outputSt.push(inputSt.top());
			buffer.push(inputSt.top());
			inputSt.pop();
		}

		buffer.pop();
		while (!(buffer.empty())) {
			inputSt.push(buffer.top());
			buffer.pop();
		}
	}
public:
	/*
	* Description:
	* Time complexity:
	* Space complexity:
	*/
	void enqueue(int v) {
		inputSt.push(v);
	}

	/*
	* Description:
	* Time complexity:
	* Space complexity:
	*/
	int dequeue() {
		if (inputSt.empty())
			return -1;

		int val;

		OutputReady();
		val = outputSt.top();

		while (!(outputSt.empty()))
			outputSt.pop();

		return val;
	}
};

int main(int argc, const char *argv[]) {
	Queue queue;
	string line;
	int count;
	bool retry = false;

	do {
		getline(cin, line);

		try {
			count = stoi(line);
			retry = false;
		}
		catch (...) { retry = true; }
	} while (retry);

	for (int i = 0; i < count; ++i) {
		getline(cin, line);
		stringstream ss(line);
		string token;
		getline(ss, token, ' ');

		if (token == "ENQUEUE") {
			getline(ss, token, ' ');
			try { queue.enqueue(stoi(token)); }
			catch (...) {
				i--;
				continue;
			}
		}
		else if (token == "DEQUEUE") {
			int val = queue.dequeue();
			if (val == -1) {
				cout << "큐가 Empty 입니다" << endl;
				i--;
				continue;
			}
			cout << val << endl;
		}
		else {
			i--;
			continue;
		}
	}

	return 0;
}