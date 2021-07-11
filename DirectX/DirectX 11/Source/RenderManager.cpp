#include "RenderManager.h"
#include <d3d11.h>
#include <vector>
#include "BumpMapShader.h"
#include "Camera.h"
#include "D3DManager.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "TextureArray.h"

bool RenderManager::Initialize(HWND hWnd, SIZE size)
{
	d3d = new D3DManager{};
	if (!d3d) return false;

	bool result = d3d->Initialize(hWnd, size, VSYNC_ENABLED, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result) return false;

	shader = new BumpMapShader{};
	if (!shader) return false;

	result = shader->Initialize(d3d->GetDevice(), hWnd);
	if (!result) return false;

	camera = new Camera{};
	if (!camera) return false;

	camera->SetPos(0.0f, 0.0f, -1.5f);

	model = new Model{};
	if (!model) return false;

	std::vector<std::filesystem::path> texturePaths{
		TEXT("Asset/stone.dds"), TEXT("Asset/bump.dds")};

	result = model->Initialize(d3d->GetDevice(), TEXT("Asset/Cube.mdl"), texturePaths);
	if (!result) return false;

	light = new DirectionalLight;
	if (!light) return false;

	light->SetDiffuseColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	light->SetDirection({ 0.0f, 0.0f, 1.0f });

	return true;
}

bool RenderManager::Frame()
{
	const auto context = d3d->GetDeviceContext();

	const auto view = camera->GetWorldMatrix();
	const auto proj = DirectX::XMLoadFloat4x4(&d3d->GetProjectionMatrix());

	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	static float rotation = 0.0f;

	rotation += DirectX::XM_PI * 0.0025f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	model->SetRot({ 0.0f, rotation, 0.0f });

	model->ReadyToRender(context);

	const bool result = shader->Render(context, model->GetIndexCount(),
		model->GetTextures(), model->GetWorldMatrix(), view, proj,
		light->GetDirection(), light->GetDiffuseColor());

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

	if (camera)
	{
		delete camera;
		camera = nullptr;
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

	if (d3d)
	{
		d3d->Release();
		delete d3d;
		d3d = nullptr;
	}
}
