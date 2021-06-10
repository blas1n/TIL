#pragma once

#include <DirectXMath.h>

class Frustum final
{
public:
	Frustum(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection, float screenDepth);

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float raduis);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float raduis);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	DirectX::XMFLOAT4 planes[6];
};
