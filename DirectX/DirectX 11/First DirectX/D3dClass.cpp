#include "D3dClass.h"

D3DClass::D3DClass()
	: m_swapChain(nullptr),
	m_device(nullptr),
	m_deviceContext(nullptr),
	m_renderTargetView(nullptr),
	m_depthStencilBuffer(nullptr),
	m_depthStencilState(nullptr),
	m_depthStencilView(nullptr),
	m_rasterState(nullptr) {}

D3DClass::D3DClass(const D3DClass& other) {}

D3DClass::~D3DClass() {}

bool D3DClass::Init(int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullScreen, float screenDepth, float screenNear) {
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	UINT numModes, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* pBackBuf;
	D3D11_TEXTURE2D_DESC depthBufDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	m_vsync_enabled = vsync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	if (FAILED(result)) return false;

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) return false;

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	for (UINT i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (UINT)screenWidth) {
			if (displayModeList[i].Height == (UINT)screenHeight) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1204 / 1204);
	
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error) return false;

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync_enabled) {
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

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result)) return false;

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuf));
	if (FAILED(result)) return false;

	result = m_device->CreateRenderTargetView(pBackBuf, NULL, &m_renderTargetView);
	if (FAILED(result)) return false;

	pBackBuf->Release();
	pBackBuf = nullptr;

	memset(&depthBufDesc, 0, sizeof(depthBufDesc));

	depthBufDesc.Width = screenWidth;
	depthBufDesc.Height = screenHeight;
	depthBufDesc.MipLevels = 1;
	depthBufDesc.ArraySize = 1;
	depthBufDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufDesc.SampleDesc.Count = 1;
	depthBufDesc.SampleDesc.Quality = 0;
	depthBufDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufDesc.CPUAccessFlags = 0;
	depthBufDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result)) return false;

	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result)) return false;

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result)) return false;

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result)) return false;

	m_deviceContext->RSSetState(m_rasterState);

	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	fieldOfView = XM_PIDIV4;
	screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	m_worldMatrix = XMMatrixIdentity();
	m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenDepth);

	return true;
}

void D3DClass::ShutDown() {
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, NULL);

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

void D3DClass::BeginScene(float r, float g, float b, float a) {
	float color[4]{ r, g, b, a };

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene() {
	if (m_vsync_enabled)
		m_swapChain->Present(1, 0);
	else
		m_swapChain->Present(0, 0);
}

ID3D11Device* D3DClass::GetDevice() {
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext() {
	return m_deviceContext;
}

XMMATRIX D3DClass::GetProjectionMatrix() {
	return m_projectionMatrix;
}

XMMATRIX D3DClass::GetWorldMatrix() {
	return m_worldMatrix;
}

XMMATRIX D3DClass::GetOrthoMatrix() {
	return m_orthoMatrix;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory) {
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}
