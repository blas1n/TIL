#pragma once

#ifndef __COLORSHADERCLASS_H__
#define __COLORSHADERCLASS_H__

#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

class ColorShaderClass {
private:
	struct MatrixBufferType {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	bool Init(ID3D11Device*, HWND);
	void Release();
	bool Render(ID3D11DeviceContext*, int, DirectX::CXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX);

private:
	bool InitShader(ID3D11Device*, HWND, PCTSTR, PCTSTR);
	void ReleaseShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, PCTSTR);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif