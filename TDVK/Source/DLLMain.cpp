#include "Utilities/Console.h"
#include "Pointers.h"
#include <exception>
#include "Hooks.h"

DWORD WINAPI StartRoutine(HMODULE hModule) {
	::Utilities::Console::Initialize();

	CORE_INFO("Console System Initialized");

	Pointers::hModule = hModule;
	Pointers::moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

	CORE_INFO("Module Base: {}", fmt::ptr(&Pointers::moduleBase));
	Hooks::InitializeSwapBuffer();

	while (!GetAsyncKeyState(VK_END)) {
		
	}

	::Utilities::Console::Close();

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
		FreeLibraryAndExitThread(hModule, 0);
		break;
	default: break;
	}

	return TRUE;
}