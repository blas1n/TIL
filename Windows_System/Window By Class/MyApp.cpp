#include "MyApp.h"

MyApp::MyApp(HINSTANCE hIn) {
	m_pClass = new MyWndClass(hIn);
	m_pMainWindow = new Window(m_pClass->GetName(), TEXT("My Win"), hIn);
}

MyApp::~MyApp() {
	delete m_pClass;
	delete m_pMainWindow;
}

void MyApp::Run() {
	MSG msg = {};

	m_pMainWindow->Show(true);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}