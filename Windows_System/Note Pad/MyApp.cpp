#include "MyApp.h"

MyApp::MyApp(HINSTANCE hIn) {
	m_pClass = new WinProc(hIn);
	m_pWinMain = new Window(m_pClass->GetName(), TEXT("Note Pad"), hIn);
}

MyApp::~MyApp() {
	delete m_pClass;
	delete m_pWinMain;
}

void MyApp::Run() {
	MSG msg = {};

	m_pWinMain->Show(true);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}