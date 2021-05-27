#include "RenderManager.h"
#include <d3d11.h>
#include "Camera.h"
#include "D3DManager.h"
#include "DirectionalLight.h"
#include "LightShader.h"
#include "Model.h"
#include "Texture.h"

bool RenderManager::Initialize(HWND hWnd, POINT size)
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
	
	camera->SetPos(0.0f, 0.0f, -5.0f);

	shader = new LightShader{};
	if (!shader) return false;

	result = shader->Initialize(d3d->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, CouldNotInitShader, Error, MB_OK);
		return false;
	}

	light = new DirectionalLight{};
	if (!light) return false;

	light->SetDiffuseColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	light->SetDirection({ 0.0f, 0.0f, 1.0f });

	model = new Model{};
	if (!model) return false;

	result = model->Initialize(d3d->GetDevice(),
		TEXT("Asset/Cube.txt"), TEXT("Asset/seafloor.dds"));

	if (!result)
	{
		MessageBox(hWnd, CouldNotInitModel, Error, MB_OK);
		return false;
	}

	return true;
}

bool RenderManager::Frame()
{
	static float rot = 0.0f;
	rot += DirectX::XM_PI * 0.01f;
	if (rot >= 360.0f)
		rot -= 360.0f;

	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	auto world = DirectX::XMLoadFloat4x4(&d3d->GetWorldMatrix());
	world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationY(rot));
	
	const auto view = camera->GetViewMatrix();
	const auto projection = DirectX::XMLoadFloat4x4(&d3d->GetProjectionMatrix());

	const auto context = d3d->GetDeviceContext();
	model->ReadyToRender(context);

	const bool result = shader->Render(context, model->GetIndexCount(),
		model->GetTexture(), world, view, projection, light->GetDirection(), light->GetDiffuseColor());

	if (!result) return false;

	d3d->EndScene();

	return true;
}

void RenderManager::Release() noexcept
{
	if (model)
	{
		model->Release();
		delete model;
		model = nullptr;
	}

	if (light)
	{
		delete light;
		light = nullptr;
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
