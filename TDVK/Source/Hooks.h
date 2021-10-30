#pragma once
#include <Windows.h>
#include "Pointers.h"
#include "detours.h"

namespace Hooks {
	void InitializeSwapBuffer() {
		HMODULE OpenGL = GetModuleHandle(L"opengl32.dll");
		Pointers::swapBuffers = (Functions::swapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)Pointers::swapBuffers, Functions::d_SwapBuffers);
	}
}