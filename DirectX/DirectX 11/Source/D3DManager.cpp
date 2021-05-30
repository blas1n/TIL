#include "D3DManager.h"
#include <vector>
#include <cstring>
#include <dxgi.h>
#include <d3d11.h>
#include <fstream>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

bool D3DManager::Initialize(HWND hWnd, SIZE size,
	bool isVsyncEnable, bool isFullScreen, float screenFar, float screenNear)
{
	isVsyncEnabled = isVsyncEnable;

	IDXGIFactory* factory;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	if (FAILED(result)) return false;

	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	UINT numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) return false;

	auto displayModeList = new DXGI_MODE_DESC[numModes];

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	UINT numerator = 0, denominator = 0;

	for (UINT i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == static_cast<UINT>(size.cx) && displayModeList[i].Height == static_cast<UINT>(size.cy))
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	(void)lstrcpyn(videoCardDescription, adapterDesc.Description, 128);

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = size.cx;
	swapChainDesc.BufferDesc.Height = size.cy;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (isVsyncEnable)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !isFullScreen;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	const auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	UINT createFlag = 0;

#ifdef _DEBUG
	createFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlag,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext);

	if (FAILED(result)) return false;

	ID3D11Texture2D* backBufferPtr;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr);
	if (FAILED(result)) return false;

	result = device->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView);
	if (FAILED(result)) return false;

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	memset(&depthBufferDesc, 0, sizeof(depthBufferDesc));

	depthBufferDesc.Width = size.cx;
	depthBufferDesc.Height = size.cy;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result)) return false;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result)) return false;

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	depthStencilDesc.DepthEnable = false;
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthDisableStencilState);
	if (FAILED(result)) return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result)) return false;

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result)) return false;

	deviceContext->RSSetState(rasterState);

	D3D11_VIEWPORT viewport;

	viewport.Width = static_cast<float>(size.cx);
	viewport.Height = static_cast<float>(size.cy);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);

	const float fieldOfView = DirectX::XM_PI * 0.5f;
	const float screenAspect = static_cast<float>(size.cx) / static_cast<float>(size.cy);

	DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenFar));
	DirectX::XMStoreFloat4x4(&orthoMatrix, DirectX::XMMatrixOrthographicLH(
		static_cast<float>(size.cx), static_cast<float>(size.cy), screenNear, screenFar));

#if UNICODE
	std::wofstream outFile{ L"GPU Info.txt" };
#else
	std::ofstream outFile{ "GPU Info.txt" };
#endif

	outFile << "Name : " << videoCardDescription << std::endl;
	outFile << "Memeory Capacity : " << videoCardMemory << std::endl;

	outFile.close();

	return true;
}

void D3DManager::Release() noexcept
{
	if (swapChain)
		swapChain->SetFullscreenState(false, nullptr);

	if (rasterState)
	{
		rasterState->Release();
		rasterState = nullptr;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	if (depthDisableStencilState)
	{
		depthDisableStencilState->Release();
		depthDisableStencilState = nullptr;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}
}

void D3DManager::BeginScene(float r, float g, float b, float a)
{
	const float color[4]{ r, g, b, a };
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DManager::EndScene()
{
	swapChain->Present(isVsyncEnabled ? 1 : 0, 0);
}

void D3DManager::GetVideoCardInfo(PTSTR cardName, int& memory)
{
	(void)lstrcpyn(cardName, videoCardDescription, 128);
	memory = videoCardMemory;
}

void D3DManager::EnableZBuffer()
{
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}

void D3DManager::DisableZBuffer()
{
	deviceContext->OMSetDepthStencilState(depthDisableStencilState, 1);
}
