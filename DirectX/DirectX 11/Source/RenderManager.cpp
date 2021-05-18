#include "GraphicsClass.h"

bool GraphicsClass::Init(std::tuple<int, int> screenSize, HWND hWnd) {
	auto screenWidth = 0, screenHeight = 0;

	std::tie(screenWidth, screenHeight) = std::move(screenSize);

	m_D3D = std::make_unique<D3dClass>();
	auto result = m_D3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	
	if (!result) {
		MessageBox(hWnd, TEXT("Could not initialize Direct3D"), TEXT("Error"), MB_OK);
		return false;
	}

	m_camera = std::make_unique<CameraClass>();
	if (!m_camera.get()) return false;

	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_model = std::make_unique<ModelClass>();
	if (!m_model.get()) return false;

	result = m_model->Init(m_D3D->GetDevice());
	if (!result) {
		MessageBox(hWnd, TEXT("Could not initialize the model object"), TEXT("Error"), MB_OK);
		return false;
	}

	m_colorShader = std::make_unique<ColorShaderClass>();
	if (!m_colorShader.get()) return false;

	result = m_colorShader->Init(m_D3D->GetDevice(), hWnd);
	if (!result) {
		MessageBox(hWnd, TEXT("Could not initalize the color shader object"), TEXT("Error"), MB_OK);
		return false;
	}
	
	return true;
}

void GraphicsClass::Release() {
	if (m_colorShader.get())
		m_colorShader->Release();

	if (m_model.get())
		m_model->Release();

	if (m_D3D.get())
		m_D3D->Release();
}

bool GraphicsClass::Frame() {
	return Render();
}

bool GraphicsClass::Render() {
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_camera->Render();

	auto viewMatrix = m_camera->GetViewMatrix();
	auto worldMatrix = m_D3D->GetWorldMatrix();
	auto projectionMatrix = m_D3D->GetProjectionMatrix();
	
	m_model->Render(m_D3D->GetDeviceContext());
	
	auto result = m_colorShader->Render(m_D3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result) return false;

	m_D3D->EndScene();

	return true;
}