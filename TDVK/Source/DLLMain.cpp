#define _WIN32_WINNT 0x0500
#include "Utilities/Console.h"
#include "Interface/Menu.h"
#include "Interface/Console.h"
#include "SwapBuffers.h"
#include "Global.h"

DWORD WINAPI StartRoutine(HMODULE hModule) {
	::Utilities::Console::Initialize();

	CORE_INFO("Console System Initialized");

	Pointers::hModule = hModule;
	Pointers::moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	Pointers::gWindow = FindWindow(nullptr, L"Teardown");

	InitializeSwapBufferHook();
	InitializeHIDHook();

	CMenu Menu;
	CConsole Console;
	Pointers::renderFunctions.push_back(std::bind(&CMenu::Render, &Menu));
	Pointers::renderFunctions.push_back(std::bind(&CConsole::Render, &Console));

	while (!GetAsyncKeyState(VK_END)) {

	}

	::Utilities::Console::Close();

	DestroyHIDHook();
	DestroySwapBufferHook();

	FreeLibraryAndExitThread(hModule, 0);

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartRoutine), hModule, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}

	return TRUE;
}