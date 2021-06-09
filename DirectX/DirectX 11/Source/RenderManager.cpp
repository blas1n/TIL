#include "RenderManager.h"
#include <d3d11.h>
#include "Camera.h"
#include "D3DManager.h"
#include "ObjParser.h"
#include "Text.h"

bool RenderManager::Initialize(HWND hWnd, SIZE size)
{
	constexpr static auto Error = TEXT("Error");
	constexpr static auto CouldNotInitD3D = TEXT("Could not initialize Direct3D");
	constexpr static auto CouldNotInitModel = TEXT("Could not initialize the model object.");
	constexpr static auto CouldNotInitShader = TEXT("Could not initialize the shader object.");

	d3d = new D3DManager{};
	if (!d3d) return false;

	bool result = d3d->Initialize(hWnd, size, VSYNC_ENABLED, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, CouldNotInitD3D, Error, MB_OK);
		return false;
	}

	camera = new Camera{};
	if (!camera) return false;

	camera->SetPos(0.0f, 0.0f, -1.0f);
	const auto baseViewMat = camera->GetWorldMatrix();
	
	DirectX::XMFLOAT4X4 viewMat;
	DirectX::XMStoreFloat4x4(&viewMat, baseViewMat);

	text = new Text{};
	if (!text) return false;

	result = text->Initialize(d3d->GetDevice(), d3d->GetDeviceContext(), hWnd, size, viewMat);
	if (!result)
	{
		MessageBox(hWnd, CouldNotInitModel, Error, MB_OK);
		return false;
	}

	return true;
}

bool RenderManager::Frame(int fps, int cpu, float frameTime)
{
	const auto context = d3d->GetDeviceContext();
	bool result = text->SetFps(context, fps);
	if (!result) return false;

	result = text->SetCpu(context, cpu);
	if (!result) return false;

	camera->SetPos(0.0f, 0.0f, -10.0f);

	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	const auto world = DirectX::XMMatrixIdentity();
	const auto ortho = DirectX::XMLoadFloat4x4(&d3d->GetOrthoMatrix());

	d3d->DisableZBuffer();
	d3d->EnableAlphaBlending();

	text->Render(context, world, ortho);
	if (!result) return false;

	d3d->DisableAlphaBlending();
	d3d->EnableZBuffer();

	d3d->EndScene();

	return true;
}

void RenderManager::Release() noexcept
{
	if (text)
	{
		text->Release();
		delete text;
		text = nullptr;
	}

	if (camera)
	{
		delete camera;
		camera = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		delete d3d;
		d3d = nullptr;
	}
}
