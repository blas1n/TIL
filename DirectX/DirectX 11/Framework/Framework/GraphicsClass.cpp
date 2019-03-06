#include "GraphicsClass.h"

bool GraphicsClass::Init(std::tuple<int, int> screenSize, HWND hWnd) {
	int screenWidth = 0, screenHeight = 0;

	std::tie(screenWidth, screenHeight) = std::move(screenSize);

	m_D3D = std::make_unique<D3dClass>();
	bool result = m_D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	
	if (!result)
		MessageBox(hWnd, TEXT("Could not initialize Direct3D"), TEXT("Error"), MB_OK);

	return result;
}

void GraphicsClass::Release() {
	if (m_D3D.get())
		m_D3D->Release();
}

bool GraphicsClass::Frame() {
	return Render();
}

bool GraphicsClass::Render() {
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_D3D->EndScene();

	return true;
}