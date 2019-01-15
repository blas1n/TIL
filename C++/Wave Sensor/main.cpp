#include <iostream>
#include <cmath>

using namespace std;

int Check(const int distance, const int order, int result);

class Sensor
{
private:
	int x, y;
public:
	Sensor() : x(0), y(0)
	{}

	Sensor(const int inputX, const int inputY)
		: x(inputX), y(inputY)
	{}

	void operator=(const Sensor& temp)
	{
		x = temp.x;
		y = temp.y;
	}

	int GetX()
	{
		return x;
	}

	int GetY()
	{
		return y;
	}
};

class Wave
{
private:
	int x, y, r;
public:
	Wave(const int inputX, const int inputY, const int inputR)
		: x(inputX), y(inputY), r(inputR)
	{}

	int Generate(Sensor& sensor)
	{
		int distance, deltaX, deltaY;

		deltaX = x - sensor.GetX();
		deltaY = y - sensor.GetY();
		distance = (int)sqrt(pow(deltaX, 2) + pow(deltaY, 2));

		if (r >= distance)
			return distance;
		else
			return -1;
	}
};

int main()
{
	int x, y, r, distance, result, i;
	result = -1;
	cin >> x >> y >> r;

	Wave wave(x, y, r);
	Sensor sensor[5];

	for (i = 0; i < 5; i++)
	{
		cin >> x >> y;

		sensor[i] = Sensor(x, y);
		distance = wave.Generate(sensor[i]);

		if (distance != -1)
			result = Check(distance, i, result);
	}

	cout << result << endl;
	system("pause");
	return 0;
}

int Check(const int distance, const int order, int result)
{
	static int min = 99999999;

	if (distance < min)
	{
		min = distance;
		result = order + 1;
	}

	return result;
}