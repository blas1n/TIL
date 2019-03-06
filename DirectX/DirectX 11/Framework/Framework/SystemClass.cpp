#include "SystemClass.h"

SystemClass& SystemClass::GetInstance() {
	static auto pInstance = new SystemClass();
	return *pInstance;
}

int SystemClass::Run() {
	if (!Init()) return 0;

	MSG msg;

	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT || !Frame()) break;
	}

	Release();
	return msg.wParam;
}

bool SystemClass::Frame() {
	return !m_input->IsKeyDown(VK_ESCAPE) && m_graphics->Frame();
}

bool SystemClass::Init() {
	auto screenSize = InitWindows();

	m_input = std::make_unique<InputClass>();
	
	if (!m_input.get())
		return false;
	
	m_graphics = std::make_unique<GraphicsClass>();

	if (!m_graphics.get())
		return false;

	return m_graphics->Init(screenSize, m_hWnd);
}

void SystemClass::Release() {
	if (m_graphics.get())
		m_graphics->Release();

	ReleaseWindows();
}

std::tuple<int, int> SystemClass::InitWindows() {
	m_hInstance = GetModuleHandle(nullptr);
	m_appName = TEXT("Engine");

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_appName;
	
	RegisterClassEx(&wc);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX, posY;

	if (FULL_SCREEN) {
		DEVMODE dmScreenSettings = { 0, };

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<ULONG>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<ULONG>(screenHeight);
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
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	ShowCursor(false);

	return std::make_tuple(screenWidth, screenHeight);
}

void SystemClass::ReleaseWindows() {
	ShowCursor(true);

	if (FULL_SCREEN)
		ChangeDisplaySettings(nullptr, 0);

	DestroyWindow(m_hWnd);
	UnregisterClass(m_appName, m_hInstance);
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN:
		m_input->KeyDown((UINT)wParam);
		return 0;

	case WM_KEYUP:
		m_input->KeyUp((UINT)wParam);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK SystemClass::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY: case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return SystemClass::GetInstance().MessageHandler(hWnd, msg, wParam, lParam);
	}
}