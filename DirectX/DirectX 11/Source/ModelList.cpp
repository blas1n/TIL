#include "ModelList.h"
#include <random>

bool ModelList::Initialize(int modelNum)
{
	models.resize(modelNum);

	std::random_device rd;
	std::mt19937 gen{ rd() };

	std::uniform_real_distribution<float> dis{ 0.0f, 1.0f };

	for (auto& model : models)
	{
		model.color = { dis(gen), dis(gen), dis(gen), 1.0f };
		model.x = dis(gen) * 10.0f;
		model.y = dis(gen) * 10.0f;
		model.z = dis(gen) * 10.0f + 5.0f;
	}

	return true;
}
