#pragma once

#include <DirectXMath.h>

struct DirectionalLight final
{
public:
	void SetAmbientColor(const DirectX::XMFLOAT4& inAmbientColor) noexcept { diffuseColor = inAmbientColor; }
	void SetDiffuseColor(const DirectX::XMFLOAT4& inDiffuseColor) noexcept { diffuseColor = inDiffuseColor; }
	void SetDirection(const DirectX::XMFLOAT3& inDirection) noexcept { direction = inDirection; }

	const DirectX::XMFLOAT4& GetAmbientColor() const noexcept { return ambientColor; }
	const DirectX::XMFLOAT4& GetDiffuseColor() const noexcept { return diffuseColor; }
	const DirectX::XMFLOAT3& GetDirection() const noexcept { return direction; }

private:
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 direction;
};
