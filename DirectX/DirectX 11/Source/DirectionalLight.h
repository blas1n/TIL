#pragma once

#include <DirectXMath.h>

struct DirectionalLight final
{
public:
	void SetAmbientColor(const DirectX::XMFLOAT4& inAmbientColor) noexcept { ambientColor = inAmbientColor; }
	void SetDiffuseColor(const DirectX::XMFLOAT4& inDiffuseColor) noexcept { diffuseColor = inDiffuseColor; }
	void SetSpecularColor(const DirectX::XMFLOAT4& inSpecularColor) noexcept { specularColor = inSpecularColor; }
	void SetDirection(const DirectX::XMFLOAT3& inDirection) noexcept { direction = inDirection; }
	void SetSpecularPower(float inSpecularPower) noexcept { specularPower = inSpecularPower; }

	const DirectX::XMFLOAT4& GetAmbientColor() const noexcept { return ambientColor; }
	const DirectX::XMFLOAT4& GetDiffuseColor() const noexcept { return diffuseColor; }
	const DirectX::XMFLOAT4& GetSpecularColor() const noexcept { return specularColor; }
	const DirectX::XMFLOAT3& GetDirection() const noexcept { return direction; }
	float GetSpecularPower() const noexcept { return specularPower; }

private:
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT4 specularColor;
	DirectX::XMFLOAT3 direction;
	float specularPower;
};
