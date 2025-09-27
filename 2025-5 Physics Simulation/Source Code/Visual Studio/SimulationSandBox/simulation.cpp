#include "pch.h"
#include "ThreadManager.h"
#include "D3DFramework.h"
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
using namespace helpers;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	HANDLE mainHandle = GetCurrentThread();
	SetThreadAffinityMask(mainHandle, PROCESSOR1);

	auto app = D3DFramework::getInstance();
	if (FAILED(app->initWindow(hInstance, nShowCmd)))
		return 0;

	auto threadManager = ThreadManager::getInstance();
	threadManager->start();

	// Main message loop
	MSG msg;
	msg.message = 0;
	while (WM_QUIT != msg.message && threadManager->threadsActive()) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);
}