#include "Frustum.h"

void Frustum::ConstructFrustum(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection, float screenDepth)
{
	DirectX::XMFLOAT4X4 projMat;
	DirectX::XMStoreFloat4x4(&projMat, projection);

	float zMinimum = -projMat._43 / projMat._33;
	float r = screenDepth / (screenDepth - zMinimum);

	projMat._33 = r;
	projMat._43 = -r * zMinimum;

	DirectX::XMMATRIX mat = DirectX::XMMatrixMultiply(view, DirectX::XMLoadFloat4x4(&projMat));
	DirectX::XMFLOAT4X4 value;
	DirectX::XMStoreFloat4x4(&value, mat);

	CalcPlane(0, value._14 + value._13, value._24 + value._23, value._34 + value._33, value._44 + value._43);
	CalcPlane(1, value._14 - value._13, value._24 - value._23, value._34 - value._33, value._44 - value._43);
	CalcPlane(2, value._14 + value._11, value._24 + value._21, value._34 + value._31, value._44 + value._41);
	CalcPlane(3, value._14 - value._11, value._24 - value._21, value._34 - value._31, value._44 - value._41);
	CalcPlane(4, value._14 - value._12, value._24 - value._22, value._34 - value._32, value._44 - value._42);
	CalcPlane(5, value._14 + value._12, value._24 + value._22, value._34 + value._32, value._44 + value._42);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	const DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 0.0f);

	for (const auto& plane : planes)
	{
		const DirectX::XMVECTOR planeVec = DirectX::XMLoadFloat4(&plane);
		const DirectX::XMVECTOR dot = DirectX::XMPlaneDotCoord(planeVec, pos);
		
		if (DirectX::XMVectorGetX(dot) < 0.0f)
			return false;
	}

	return true;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	const DirectX::XMVECTOR pos0 = DirectX::XMVectorSet(xCenter - radius, yCenter - radius, zCenter - radius, 0.0f);
	const DirectX::XMVECTOR pos1 = DirectX::XMVectorSet(xCenter + radius, yCenter - radius, zCenter - radius, 0.0f);
	const DirectX::XMVECTOR pos2 = DirectX::XMVectorSet(xCenter - radius, yCenter + radius, zCenter - radius, 0.0f);
	const DirectX::XMVECTOR pos3 = DirectX::XMVectorSet(xCenter + radius, yCenter + radius, zCenter - radius, 0.0f);
	const DirectX::XMVECTOR pos4 = DirectX::XMVectorSet(xCenter - radius, yCenter - radius, zCenter + radius, 0.0f);
	const DirectX::XMVECTOR pos5 = DirectX::XMVectorSet(xCenter + radius, yCenter - radius, zCenter + radius, 0.0f);
	const DirectX::XMVECTOR pos6 = DirectX::XMVectorSet(xCenter - radius, yCenter + radius, zCenter + radius, 0.0f);
	const DirectX::XMVECTOR pos7 = DirectX::XMVectorSet(xCenter + radius, yCenter + radius, zCenter + radius, 0.0f);

	for (const auto& plane : planes)
	{
		const DirectX::XMVECTOR planeVec = DirectX::XMLoadFloat4(&plane);

		DirectX::XMVECTOR dot = DirectX::XMPlaneDotCoord(planeVec, pos0);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos0);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos1);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos2);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos3);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos4);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos5);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos6);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos7);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		return false;
	}

	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	const DirectX::XMVECTOR pos = DirectX::XMVectorSet(xCenter, yCenter, zCenter, 0.0f);

	for (const auto& plane : planes)
	{
		const DirectX::XMVECTOR planeVec = DirectX::XMLoadFloat4(&plane);
		const DirectX::XMVECTOR dot = DirectX::XMPlaneDotCoord(planeVec, pos);

		if (DirectX::XMVectorGetX(dot) < -radius)
			return false;
	}

	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	const DirectX::XMVECTOR pos0 = DirectX::XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter - zSize, 0.0f);
	const DirectX::XMVECTOR pos1 = DirectX::XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter - zSize, 0.0f);
	const DirectX::XMVECTOR pos2 = DirectX::XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter - zSize, 0.0f);
	const DirectX::XMVECTOR pos3 = DirectX::XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter - zSize, 0.0f);
	const DirectX::XMVECTOR pos4 = DirectX::XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter + zSize, 0.0f);
	const DirectX::XMVECTOR pos5 = DirectX::XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter + zSize, 0.0f);
	const DirectX::XMVECTOR pos6 = DirectX::XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter + zSize, 0.0f);
	const DirectX::XMVECTOR pos7 = DirectX::XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter + zSize, 0.0f);

	for (const auto& plane : planes)
	{
		const DirectX::XMVECTOR planeVec = DirectX::XMLoadFloat4(&plane);

		DirectX::XMVECTOR dot = DirectX::XMPlaneDotCoord(planeVec, pos0);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos0);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos1);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos2);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos3);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos4);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos5);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos6);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		dot = DirectX::XMPlaneDotCoord(planeVec, pos7);
		if (DirectX::XMVectorGetX(dot) >= 0.0f)
			continue;

		return false;
	}

	return true;
}

void Frustum::CalcPlane(int idx, float x, float y, float z, float w)
{
	planes[idx].x = x;
	planes[idx].y = y;
	planes[idx].z = z;
	planes[idx].w = w;

	DirectX::XMVECTOR plane = DirectX::XMLoadFloat4(&planes[idx]);
	DirectX::XMStoreFloat4(&planes[idx], DirectX::XMPlaneNormalize(plane));
}
