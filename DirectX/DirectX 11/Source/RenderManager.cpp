#include "RenderManager.h"
#include "D3DManager.h"

constexpr static float SCREEN_FAR = 1000.0f;
constexpr static float SCREEN_NEAR = 0.1f;
constexpr static bool VSYNC_ENABLED = true;
constexpr static bool FULL_SCREEN = true;

bool RenderManager::Initialize(HWND hWnd, POINT size)
{
	d3d = new D3DManager{};
	bool result = d3d->Initialize(hWnd, size, VSYNC_ENABLED, FULL_SCREEN, SCREEN_FAR, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, TEXT("Could not initialize Direct3D"), TEXT("Error"), MB_OK);
		return false;
	}

	return true;
}

bool RenderManager::Frame()
{
	d3d->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	d3d->EndScene();

	return true;
}

void RenderManager::Release()
{
	if (d3d)
	{
		d3d->Release();
		delete d3d;
		d3d = nullptr;
	}
}
