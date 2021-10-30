#pragma once

#include "Global.h"
#include <vector>

namespace Pointers {
	uintptr_t moduleBase;
	HMODULE hModule;
	HWND gWindow;
	Functions::twglSwapBuffers owglSwapBuffers;
	Functions::hk_SetCursorPos ocursor;
	bool showMenu;
	bool showConsole;
	std::vector<renderFunction> renderFunctions;
}