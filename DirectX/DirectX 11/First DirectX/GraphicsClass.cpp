#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
	: m_D3D(nullptr) {}

GraphicsClass::GraphicsClass(const GraphicsClass& other) {}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hWnd) {
	m_D3D = new D3DClass();
	if (!m_D3D) return false;

	bool result = m_D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
		MessageBox(hWnd, "Could not init Direct3D", "Error", MB_OK);

	return result;
}

void GraphicsClass::ShutDown() {
	if (m_D3D) {
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = nullptr;
	}
}


bool GraphicsClass::Frame() {
	return Render();
}

bool GraphicsClass::Render() {
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	m_D3D->EndScene();

	return true;
}
