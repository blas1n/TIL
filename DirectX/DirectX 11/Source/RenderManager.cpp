#include "RenderManager.h"
#include <d3d11.h>
#include "Camera.h"
#include "D3DManager.h"
#include "Bitmap.h"
#include "ObjParser.h"
#include "Texture.h"
#include "TextureShader.h"

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
	
	camera->SetPos(0.0f, 0.0f, -2.0f);

	shader = new TextureShader{};
	if (!shader) return false;

	result = shader->Initialize(d3d->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, CouldNotInitShader, Error, MB_OK);
		return false;
	}

	bitmap = new Bitmap{};
	if (!bitmap) return false;

	result = bitmap->Initialize(d3d->GetDevice(), size, TEXT("Asset/seafloor.dds"), SIZE{ 256, 256 });

	if (!result)
	{
		MessageBox(hWnd, CouldNotInitModel, Error, MB_OK);
		return false;
	}

	return true;
}

bool RenderManager::Frame()
{
	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	d3d->DisableZBuffer();

	const auto context = d3d->GetDeviceContext();
	bitmap->ReadyToRender(context);
	
	const auto world = bitmap->GetWorldMatrix();
	const auto view = camera->GetViewMatrix();
	const auto ortho = DirectX::XMLoadFloat4x4(&d3d->GetOrthoMatrix());

	const bool result = shader->Render(context,
		bitmap->GetIndexCount(), bitmap->GetTexture(), world, view, ortho);

	if (!result) return false;

	d3d->EndScene();

	return true;
}

void RenderManager::Release() noexcept
{
	if (bitmap)
	{
		bitmap->Release();
		delete bitmap;
		bitmap = nullptr;
	}

	if (shader)
	{
		shader->Release();
		delete shader;
		shader = nullptr;
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
