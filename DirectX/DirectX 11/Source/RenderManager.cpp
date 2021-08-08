#include "RenderManager.h"
#include <d3d11.h>
#include <vector>
#include "Camera.h"
#include "DebugWindow.h"
#include "DirectionalLight.h"
#include "D3DManager.h"
#include "LightShader.h"
#include "Model.h"
#include "RenderTexture.h"
#include "TextureArray.h"
#include "TextureShader.h"

bool RenderManager::Initialize(HWND hWnd, SIZE size)
{
	d3d = new D3DManager{};
	if (!d3d) return false;

	bool result = d3d->Initialize(hWnd, size, VSYNC_ENABLED, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result) return false;

	light = new DirectionalLight;
	if (!light) return false;

	light->SetDiffuseColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	light->SetSpecularColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	light->SetSpecularPower(16.0f);
	light->SetDirection({ 0.0f, 0.0f, 1.0f });

	lightShader = new LightShader{};
	if (!lightShader) return false;

	result = lightShader->Initialize(d3d->GetDevice(), hWnd);
	if (!result) return false;

	textureShader = new TextureShader{};
	if (!textureShader) return false;

	result = textureShader->Initialize(d3d->GetDevice(), hWnd);
	if (!result) return false;

	camera = new Camera{};
	if (!camera) return false;

	camera->SetPos(0.0f, 0.0f, -1.5f);

	model = new Model{};
	if (!model) return false;

	std::vector<std::filesystem::path> texturePaths{ TEXT("Asset/seafloor.dds") };
	result = model->Initialize(d3d->GetDevice(), TEXT("Asset/Cube.mdl"), texturePaths);
	if (!result) return false;

	renderTexture = new RenderTexture{};
	if (!renderTexture) return false;

	result = renderTexture->Initialize(d3d->GetDevice(), size.cx, size.cy);;
	if (!result) return false;

	debugWindow = new DebugWindow;
	if (!debugWindow) return false;

	result = debugWindow->Initialize(d3d->GetDevice(), size.cx, size.cy, 100, 100);
	if (!result) return false;

	return true;
}

bool RenderManager::Frame()
{
	const auto context = d3d->GetDeviceContext();

	const auto view = camera->GetWorldMatrix();
	const auto proj = DirectX::XMLoadFloat4x4(&d3d->GetProjectionMatrix());

	bool result = RenderToTexture();
	if (!result) return false;

	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	result = RenderScene();
	if (!result) return false;

	d3d->DisableZBuffer();

	result = debugWindow->Render(context, 50, 50);
	if (!result) return false;

	result = textureShader->Render(context, model->GetIndexCount(),
		renderTexture->GetShaderResourceView(), DirectX::XMMatrixIdentity(), view, proj);

	if (!result) return false;

	d3d->EnableZBuffer();
	d3d->EndScene();
	return true;
}

void RenderManager::Release() noexcept
{
	if (debugWindow)
	{
		debugWindow->Release();
		delete debugWindow;
		debugWindow = nullptr;
	}

	if (renderTexture)
	{
		renderTexture->Release();
		delete renderTexture;
		renderTexture = nullptr;
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

	if (textureShader)
	{
		textureShader->Release();
		delete textureShader;
		textureShader = nullptr;
	}

	if (lightShader)
	{
		lightShader->Release();
		delete lightShader;
		lightShader = nullptr;
	}

	if (light)
	{
		delete light;
		light = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		delete d3d;
		d3d = nullptr;
	}
}

bool RenderManager::RenderToTexture()
{
	renderTexture->SetRenderTarget(d3d->GetDeviceContext(), d3d->GetDepthStencilView());
	renderTexture->ClearRenderTarget(d3d->GetDeviceContext(), d3d->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	if (!RenderScene()) return false;

	d3d->SetBackBufferRenderTarget();
	return true;
}

bool RenderManager::RenderScene()
{
	auto rotation = model->GetRot();
	rotation.y += DirectX::XM_PI * 0.005f;
	if (rotation.y > 360.0f) rotation.y -= 360.0f;
	
	model->SetRot(rotation);

	model->ReadyToRender(d3d->GetDeviceContext());

	const auto view = camera->GetWorldMatrix();
	const auto proj = DirectX::XMLoadFloat4x4(&d3d->GetProjectionMatrix());

	bool result = lightShader->Render(d3d->GetDeviceContext(), model->GetIndexCount(),
		model->GetTextures()->GetTextures()[0], model->GetWorldMatrix(), view, proj, *light, camera->GetPos());

	return true;
}
