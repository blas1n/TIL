#pragma once

#include <DirectXMath.h>

class Frustum final
{
public:
	void ConstructFrustum(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection, float screenDepth);

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	void CalcPlane(int idx, float x, float y, float z, float w);

private:
	DirectX::XMFLOAT4 planes[6];
};
