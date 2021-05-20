#pragma once

#include "stdafx.h"
#include <DirectXMath.h>

class D3DManager
{
	using Matrix = DirectX::XMFLOAT4X4;
	
public:
	bool Initialize(HWND hWnd, POINT size, bool isVsyncEnable, bool isFullScreen, float screenFar, float screenNear);
	void Release() noexcept;

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	struct ID3D11Device* GetDevice() const noexcept { return device; }
	struct ID3D11DeviceContext* GetDeviceContext() const noexcept { return deviceContext; }

	const Matrix& GetProjectionMatrix() const noexcept { return projectionMatrix; }
	const Matrix& GetOrthoMatrix() const noexcept { return orthoMatrix; }
	const Matrix& GetWorldMatrix() const noexcept { return worldMatrix; }

	void GetVideoCardInfo(PTSTR cardName, int& memory);

private:
	TCHAR videoCardDescription[128];

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	struct IDXGISwapChain* swapChain;
	struct ID3D11RenderTargetView* renderTargetView;
	struct ID3D11Texture2D* depthStencilBuffer;
	struct ID3D11DepthStencilState* depthStencilState;
	struct ID3D11DepthStencilView* depthStencilView;
	struct ID3D11RasterizerState* rasterState;
	Matrix projectionMatrix;
	Matrix orthoMatrix;
	Matrix worldMatrix;

	int videoCardMemory;
	bool isVsyncEnabled;
};

