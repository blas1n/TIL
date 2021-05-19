#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class D3DManager
{
	using Matrix = DirectX::XMMATRIX;

public:
	bool Initialize(HWND hWnd, POINT size, bool isVsyncEnable, bool isFullScreen, float screenFar, float screenNear);
	void Release();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice() const noexcept { return device; }
	ID3D11DeviceContext* GetDeviceContext() const noexcept { return deviceContext; }

	Matrix GetProjectionMatrix() const noexcept { return projectionMatrix; }
	Matrix GetWorldMatrix() const noexcept { return worldMatrix; }
	Matrix GetOrthoMatrix() const noexcept { return orthoMatrix; }

	void GetVideoCardInfo(PTSTR cardName, int& memory);

private:
	TCHAR videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	Matrix projectionMatrix;
	Matrix worldMatrix;
	Matrix orthoMatrix;

	int videoCardMemory;
	bool isVsyncEnabled;
};

