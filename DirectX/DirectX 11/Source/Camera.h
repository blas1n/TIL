#pragma once

#include <DirectXMath.h>

class Camera final
{
public:
	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMFLOAT3 GetPos() const noexcept { return DirectX::XMFLOAT3{ posX, posY, posZ }; }
	DirectX::XMFLOAT3 GetRot() const noexcept { return DirectX::XMFLOAT3{ rotX, rotY, rotZ }; }

	void SetPos(float x, float y, float z) noexcept { posX = x; posY = y; posZ = z; }
	void SetRot(float x, float y, float z) noexcept { rotX = x; rotY = y; rotZ = z; }

private:
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
};
