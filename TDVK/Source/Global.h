#pragma once
#include <cstdint>

#include <Windows.h>
#include <GL/glew.h>
#include <functional>

typedef std::function<void()> renderFunction;

namespace Functions {
	typedef BOOL(__stdcall* twglSwapBuffers)(_In_ HDC hdc);
	typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);
}

namespace Pointers {
	extern uintptr_t moduleBase;
	extern HMODULE hModule;
	extern HWND gWindow;
	extern Functions::twglSwapBuffers owglSwapBuffers;
	extern Functions::hk_SetCursorPos ocursor;
	extern bool showMenu;
	extern bool showConsole;
	extern std::vector<renderFunction> renderFunctions;
}