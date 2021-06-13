#include "ModelList.h"
#include <random>

bool ModelList::Initialize(int modelNum)
{
	models.resize(modelNum);

	std::random_device rd;
	std::mt19937 gen{ rd() };

	std::uniform_real_distribution<float> colorDis{ 0.0f, 1.0f };
	std::uniform_real_distribution<float> pointDis{ -10.0f, 10.0f };

	for (auto& model : models)
	{
		model.color = { colorDis(gen), colorDis(gen), colorDis(gen), 1.0f };
		model.x = pointDis(gen);
		model.y = pointDis(gen);
		model.z = colorDis(gen) * 10.0f + 5.0f;
	}

	return true;
}
