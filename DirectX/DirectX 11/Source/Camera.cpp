#include "Camera.h"

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	const auto posTemp = GetPos();
	const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&posTemp);

	DirectX::XMFLOAT3 buf{ 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&buf);
	
	buf = { 0.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&buf);

	const auto rotMat = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(rotX),
		DirectX::XMConvertToRadians(rotY),
		DirectX::XMConvertToRadians(rotZ)
	);

	up = DirectX::XMVector3TransformCoord(up, rotMat);
	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotMat);
	lookAt = DirectX::XMVectorAdd(pos, lookAt);

	return DirectX::XMMatrixLookAtLH(pos, lookAt, up);
}
