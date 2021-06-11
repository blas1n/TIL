#pragma once

#include <DirectXMath.h>
#include <vector>

struct ModelInfo final
{
	DirectX::XMFLOAT4 color;
	float x, y, z;
};

class ModelList final
{
public:
	bool Initialize(int modelNum);
	void Release() noexcept { models.clear(); }

	ModelInfo& operator[](int idx) noexcept { return models[idx]; }
	const ModelInfo& operator[](int idx) const noexcept { return models[idx]; }
	
	unsigned int GetModelCount() const noexcept { return models.size(); }

private:
	std::vector<ModelInfo> models;
};
