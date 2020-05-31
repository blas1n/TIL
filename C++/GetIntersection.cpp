#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>

using namespace std;

using Input = vector<string>;
using Inputs = vector<Input>;
using InputIter = Input::iterator;

Inputs GetInputs(const string& path)
{
	Inputs inputs;

	for (char i = '0';; ++i)
	{
		auto file = path + i + ".txt";
		if (!filesystem::exists(file)) break;

		ifstream ifs(file);
		Input input;
		
		while (!ifs.eof())
		{
			string str;
			getline(ifs, str);
			input.emplace_back(move(str));
		}

		sort(input.begin(), input.end());
		inputs.emplace_back(move(input));
	}

	return inputs;
}

Input PopVec(Inputs& inputs)
{
	auto ret = std::move(inputs.back());
	inputs.pop_back();
	return std::move(ret);
}

inline bool CompareVecSize(const Input& lhs, const Input& rhs)
{
	return lhs.size() < rhs.size();
}

int main()
{
	string path, out;
	cin >> path >> out;
	auto inputs = GetInputs(path);

	Input results(move(PopVec(inputs)));

	while (!inputs.empty())
	{
		auto input = PopVec(inputs);
		auto begin = move_iterator<InputIter>(input.begin());
		auto end = move_iterator<InputIter>(input.end());

		vector<string> result(min(input.size(), results.size()));
		auto iter = set_intersection(begin, end, results.cbegin(), results.cend(), result.begin());
		result.resize(iter - result.begin());
		results = move(result);
	}

	ofstream ofs{ out };
	
	for (const auto& result : results)
		ofs << result << endl;

	cout << "¿Ï·á" << endl;
	return 0;
}