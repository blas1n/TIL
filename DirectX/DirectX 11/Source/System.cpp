#include "System.h"
#include "RenderManager.h"
#include "InputManager.h"

namespace
{
	System* inst;
	LRESULT(System::* callback)(HWND, UINT, WPARAM, LPARAM);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return (inst->*callback)(hWnd, msg, wParam, lParam);
		}
	}
}

bool System::Init()
{
	inst = this;
	callback = &System::MessageHandler;

	const auto screenSize = InitWindows();

	input = new InputManager{};
	if (!input->Initialize(hInstance, hWnd, screenSize))
		return false;

	render = new RenderManager{};
	if (!render->Initialize(hWnd, screenSize))
		return false;

	return true;
}

int System::Run()
{
	if (!Init()) return 0;

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT || !Frame()) break;
	}

	Release();
	return static_cast<int>(msg.wParam);
}

void System::Release() noexcept
{
	render->Release();
	input->Release();
	ReleaseWindows();
}

bool System::Frame()
{
	return !input->Frame() && render->Frame();
}

POINT System::InitWindows()
{
	hInstance = GetModuleHandle(nullptr);
	appName = TEXT("Engine");

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszMenuName = appName;

	RegisterClassEx(&wc);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX, posY;

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings = { 0, };

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<ULONG>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<ULONG>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}

	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(false);

	return POINT{ screenWidth, screenHeight };
}

void System::ReleaseWindows() noexcept
{
	ShowCursor(true);

	if (FULL_SCREEN)
		ChangeDisplaySettings(nullptr, 0);

	DestroyWindow(hWnd);
	UnregisterClass(appName, hInstance);
}

LRESULT CALLBACK System::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

