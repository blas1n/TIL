#pragma once

#include "stdafx.h"
#include <DirectXMath.h>

class TextureShader final
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device, HWND hWnd);
	[[nodiscard]] bool Render(struct ID3D11DeviceContext* context, UINT indexCount,
		class Texture* texture, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection);

	void Release() noexcept;

	bool SetParameter(ID3D11DeviceContext* context, Texture* texture,
		DirectX::FXMMATRIX worldMatrix,	DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

private:
	void OutputShaderError(struct ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader);

private:
	struct ID3D11VertexShader* vertexShader = nullptr;
	struct ID3D11PixelShader* pixelShader = nullptr;
	struct ID3D11InputLayout* inputLayout = nullptr;
	struct ID3D11Buffer* matrixBuffer = nullptr;
	struct ID3D11SamplerState* samplerState = nullptr;
};
