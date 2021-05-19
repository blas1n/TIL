#include "D3DManager.h"
#include <vector>
#include <cstring>
#include <fstream>

bool D3DManager::Init(int screenWidth, int screenHeight, bool vsync, HWND hWnd,
	bool fullScreen, float screenDepth, float screenNear) {

	HRESULT result;

	m_vsync_enabled = vsync;

	IDXGIFactory* factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	if (FAILED(result)) return false;

	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	UINT numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) return false;

	auto displayModeList = new DXGI_MODE_DESC[numModes];

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	UINT numerator = 0, denominator = 0;

	for (UINT i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == static_cast<UINT>(screenWidth)) {
			if (displayModeList[i].Height == static_cast<UINT>(screenHeight)) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	lstrcpyn(m_videoCardDescription, adapterDesc.Description, 128);

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
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsync) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}

	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullScreen;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	UINT createFlag = 0;

#ifdef _DEBUG
	createFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlag,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);

	if (FAILED(result)) return false;

	ID3D11Texture2D* backBufferPtr;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr);
	if (FAILED(result)) return false;

	result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTargetView);
	if (FAILED(result)) return false;

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	memset(&depthBufferDesc, 0, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
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

	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result)) return false;

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result)) return false;

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

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

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result)) return false;

	m_deviceContext->RSSetState(m_rasterState);

	D3D11_VIEWPORT viewport;

	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	float fieldOfView = DirectX::XM_PI / 4.0f;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	m_worldMatrix = DirectX::XMMatrixIdentity();
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(screenWidth),
		static_cast<float>(screenHeight), screenNear, screenDepth);
	
#if UNICODE
	std::wofstream outFile("GPU Info.txt");
#else
	std::ofstream outFile("GPU Info.txt");
#endif

	outFile << "Name : " << m_videoCardDescription << std::endl;
	outFile << "Memeory Capacity : " << m_videoCardMemory << std::endl;

	outFile.close();

	return true;
}

void D3DManager::Release() {
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, nullptr);

	if (m_rasterState) {
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_depthStencilView) {
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
	
	if (m_depthStencilState) {
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}
	
	if (m_depthStencilBuffer) {
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}
	
	if (m_renderTargetView) {
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	
	if (m_deviceContext) {
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}
	
	if (m_device) {
		m_device->Release();
		m_device = nullptr;
	}
	
	if (m_swapChain) {
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

void D3DManager::BeginScene(float r, float g, float b, float a) {
	float color[4]{ r, g, b, a };

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DManager::EndScene() {
	if (m_vsync_enabled)
		m_swapChain->Present(1, 0);
	else
		m_swapChain->Present(0, 0);
}

ID3D11Device* D3DManager::GetDevice() {
	return m_device;
}

ID3D11DeviceContext* D3DManager::GetDeviceContext() {
	return m_deviceContext;
}

DirectX::XMMATRIX D3DManager::GetProjectionMatrix() {
	return m_projectionMatrix;
}

DirectX::XMMATRIX D3DManager::GetWorldMatrix() {
	return m_worldMatrix;
}

DirectX::XMMATRIX D3DManager::GetOrthoMatrix() {
	return m_orthoMatrix;
}

void D3DManager::GetVideoCardInfo(PTSTR cardName, int& memory) {
	lstrcpyn(cardName, m_videoCardDescription, 128);
	memory = m_videoCardMemory;
}