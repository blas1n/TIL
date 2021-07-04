#pragma once
#pragma once

#include "stdafx.h"
#include <DirectXMath.h>

class AlphaMapShader final
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device, HWND hWnd, float gammaCorrection);

	[[nodiscard]] bool Render(struct ID3D11DeviceContext* context, UINT indexCount,
		class TextureArray* textures, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection);

	void Release() noexcept;

private:
	[[nodiscard]] bool SetParameter(ID3D11DeviceContext* context, TextureArray* textures,
		DirectX::FXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

	void OutputShaderError(struct ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader);

private:
	struct ID3D11VertexShader* vertexShader = nullptr;
	struct ID3D11PixelShader* pixelShader = nullptr;
	struct ID3D11InputLayout* inputLayout = nullptr;
	struct ID3D11SamplerState* samplerState = nullptr;
	struct ID3D11Buffer* matrixBuffer = nullptr;
};
