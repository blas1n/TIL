#include "RenderManager.h"
#include <d3d11.h>
#include "Camera.h"
#include "D3DManager.h"
#include "DirectionalLight.h"
#include "Frustum.h"
#include "LightShader.h"
#include "Model.h"
#include "ModelList.h"
#include "ObjParser.h"
#include "Text.h"
#include "Texture.h"

bool RenderManager::Initialize(HWND hWnd, SIZE size)
{
	d3d = new D3DManager{};
	if (!d3d) return false;

	bool result = d3d->Initialize(hWnd, size, VSYNC_ENABLED, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result) return false;

	modelList = new ModelList{};
	if (!modelList) return false;

	frustum = new Frustum{};
	if (!frustum) return false;

	result = modelList->Initialize(25);
	if (!result) return false;

	light = new DirectionalLight{};
	if (!light) return false;

	light->SetDirection({ 0.0f, 0.0f, 1.0f });

	shader = new LightShader{};
	if (!shader) return false;

	result = shader->Initialize(d3d->GetDevice(), hWnd);
	if (!result) return false;

	camera = new Camera{};
	if (!camera) return false;

	camera->SetPos(0.0f, 0.0f, -1.0f);
	
	model = new Model{};
	if (!model) return false;

	result = model->Initialize(d3d->GetDevice(), TEXT("Asset/Sphere.mdl"), TEXT("Asset/seafloor.dds"));
	if (!result) return false;

	DirectX::XMFLOAT4X4 viewMat;
	const auto baseViewMat = camera->GetWorldMatrix();
	DirectX::XMStoreFloat4x4(&viewMat, baseViewMat);

	text = new Text{};
	if (!text) return false;

	result = text->Initialize(d3d->GetDevice(), d3d->GetDeviceContext(), hWnd, size, viewMat);
	if (!result) return false;

	return true;
}

bool RenderManager::Frame(float rotY)
{
	camera->SetPos(0.0f, 0.0f, -10.0f);
	camera->SetRot(0.0f, rotY, 0.0f);

	auto world = DirectX::XMMatrixIdentity();
	const auto view = camera->GetWorldMatrix();
	const auto ortho = DirectX::XMLoadFloat4x4(&d3d->GetOrthoMatrix());
	const auto proj = DirectX::XMLoadFloat4x4(&d3d->GetProjectionMatrix());

	frustum->ConstructFrustum(view, proj, SCREEN_FAR);
	const auto modelCount = modelList->GetModelCount();
	unsigned int renderCount = 0;

	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	d3d->DisableAlphaBlending();
	d3d->EnableZBuffer();

	const auto context = d3d->GetDeviceContext();
	const float radius = 1.0f;
	bool result = true;

	for (unsigned int i = 0; i < modelCount; ++i)
	{
		const float x = (*modelList)[i].x;
		const float y = (*modelList)[i].y;
		const float z = (*modelList)[i].z;

		const bool canRender = frustum->CheckSphere(x, y, z, radius);
		if (!canRender) continue;

		world = DirectX::XMMatrixTranslation(x, y, z);
		model->ReadyToRender(context);

		result = shader->Render(context, model->GetIndexCount(),
			model->GetTexture(), world, view, proj, *light, camera->GetPos());
		
		if (!result) return false;

		++renderCount;
	}

	result = text->SetRenderCount(context, renderCount);
	if (!result) return false;

	d3d->DisableZBuffer();
	d3d->EnableAlphaBlending();

	result = text->Render(context, world, ortho);
	if (!result) return false;

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

	if (shader)
	{
		shader->Release();
		delete shader;
		shader = nullptr;
	}

	if (light)
	{
		delete light;
		light = nullptr;
	}

	if (frustum)
	{
		delete frustum;
		frustum = nullptr;
	}

	if (modelList)
	{
		modelList->Release();
		delete modelList;
		modelList = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		delete d3d;
		d3d = nullptr;
	}
}
