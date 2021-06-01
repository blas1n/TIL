#pragma once

#include <DirectXMath.h>

class Object
{
public:
	DirectX::XMMATRIX GetWorldMatrix() const
	{
		constexpr static float Deg2Rad = DirectX::XM_PI / 180.0f;

		auto rotation = DirectX::XMLoadFloat3(&rot);
		rotation = DirectX::XMVectorMultiply(rotation, _mm_set1_ps(Deg2Rad));

		const auto rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
		const auto scaleMat = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));

		const auto scaleRot = DirectX::XMMatrixMultiply(scaleMat, rotMat);
		const auto posMat = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos));

		return DirectX::XMMatrixMultiply(scaleRot, posMat);
	}

	void SetPos(const DirectX::XMFLOAT3& inPos) noexcept { pos = inPos; }
	void SetPos(float x, float y, float z) noexcept { pos = { x, y, z }; }

	void SetRot(const DirectX::XMFLOAT3& inRot) noexcept { rot = inRot; }
	void SetRot(float roll, float pitch, float yaw) noexcept { rot = { roll, pitch, yaw }; }

	void SetScale(const DirectX::XMFLOAT3& inScale) noexcept { scale = inScale; }
	void SetScale(float x, float y, float z) noexcept { scale = { x, y, z }; }

	const DirectX::XMFLOAT3& GetPos() const noexcept { return pos; }
	const DirectX::XMFLOAT3& GetRot() const noexcept { return rot; }
	const DirectX::XMFLOAT3& GetScale() const noexcept { return scale; }

private:
	DirectX::XMFLOAT3 pos, rot;
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
};
