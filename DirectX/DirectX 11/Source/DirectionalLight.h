#pragma once

#include <DirectXMath.h>

class DirectionalLight final
{
public:
	void SetDiffuseColor(const DirectX::XMFLOAT4& inDiffuseColor) noexcept { diffuseColor = inDiffuseColor; }
	void SetDirection(const DirectX::XMFLOAT3& inDirection) noexcept { direction = inDirection; }

	const DirectX::XMFLOAT4& GetDiffuseColor() const noexcept { return diffuseColor; }
	const DirectX::XMFLOAT3& GetDirection() const noexcept { return direction; }

private:
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 direction;
};
