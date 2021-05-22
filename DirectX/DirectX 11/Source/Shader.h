#pragma once

#include "stdafx.h"
#include <DirectXMath.h>
#include <vector>

class Shader
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device, HWND hWnd);
	[[nodiscard]] bool Render(struct ID3D11DeviceContext* context, UINT indexCount,
		DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection);

	void Release() noexcept;

	bool SetParameter(ID3D11DeviceContext* context, DirectX::FXMMATRIX worldMatrix,
		DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

protected:
	virtual std::vector<struct D3D11_INPUT_ELEMENT_DESC> GetInputElements() const = 0;
	virtual std::tuple<const TCHAR*, const TCHAR*> GetShaderNames() const = 0;

private:
	void OutputShaderError(struct ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader);

private:
	struct ID3D11VertexShader* vertexShader = nullptr;
	struct ID3D11PixelShader* pixelShader = nullptr;
	struct ID3D11InputLayout* inputLayout = nullptr;
	struct ID3D11Buffer* matrixBuffer = nullptr;
};
