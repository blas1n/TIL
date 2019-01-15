#include "SystemClass.h"

SystemClass::SystemClass()
	: m_Input(nullptr), m_Graphics(nullptr) {}

SystemClass::SystemClass(const SystemClass& other) {}

SystemClass::~SystemClass() {}

bool SystemClass::Init() {
	int screenWidth, screenHeight;

	screenWidth = screenHeight = 0;

	InitWindows(screenWidth, screenHeight);

	m_Input = new InputClass();
	if (!m_Input)
		return false;

	m_Input->Init();

	m_Graphics = new GraphicsClass();
	if (!m_Graphics)
		return false;

	if (!m_Graphics->Init(screenWidth, screenHeight, m_hWnd))
		return false;

	return true;
}

void SystemClass::ShutDown() {
	if (m_Graphics) {
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = nullptr;
	}

	ShutDownWindows();
}

void SystemClass::Run() {
	MSG msg;
	bool done;

	memset(&msg, 0, sizeof(MSG));
	done = false;

	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			done = true;

		else {
			if (!Frame())
				done = true;
		}
	}
}

bool SystemClass::Frame() {
	if (m_Input->IsKeyDown(VK_ESCAPE))
		return false;

	if (!m_Graphics->Frame())
		return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
	case WM_KEYDOWN:
		m_Input->KeyDown((UINT)wparam);
		return 0;
	
	case WM_KEYUP:
		m_Input->KeyUp((UINT)wparam);
		return 0;

	default:
		return DefWindowProc(hWnd, umsg, wparam, lparam);
	}
}

void SystemClass::InitWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	hApp = this;
	m_hIn = GetModuleHandle(NULL);
	m_appName = _T("Engine");

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hIn;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (UINT)screenWidth;
		dmScreenSettings.dmPelsHeight = (UINT)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else {
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY,
		screenWidth, screenHeight, NULL, NULL, m_hIn, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	ShowCursor(false);
}

void SystemClass::ShutDownWindows() {
	ShowCursor(true);

	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	UnregisterClass(m_appName, m_hIn);
	m_hIn = NULL;
	hApp = NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return hApp->MessageHandler(hWnd, umsg, wparam, lparam);
	}
}