#pragma once

#include "Object.h"

class Camera : public Object
{
public:
	DirectX::XMMATRIX GetWorldMatrix() const override
	{
		const float radians = GetRot().y * 0.0174532925f;

		const DirectX::XMFLOAT3 eye = GetPos();
		const DirectX::XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
		
		const DirectX::XMFLOAT3 lookAt
		{
			sinf(radians) + GetPos().x,
			GetPos().y,
			cosf(radians) + GetPos().z
		};

		return DirectX::XMMatrixLookAtLH(
			DirectX::XMLoadFloat3(&eye),
			DirectX::XMLoadFloat3(&lookAt),
			DirectX::XMLoadFloat3(&up)
		);
	}
};
