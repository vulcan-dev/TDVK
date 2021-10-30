#pragma once
#include <cstdint>
#include <minwindef.h>

#include <GL/glew.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>

std::once_flag swapBuffersInit;

void OnSwapBufferInitialize() {
	if (!ImGui::GetCurrentContext()) {
		CORE_INFO("Creating ImGui Context");
		glewInit();
		ImGui::CreateContext();
	}
}

namespace Functions {
	typedef BOOL(__stdcall* swapBuffers)(_In_ HDC hdc);
	bool d_SwapBuffers(_In_ HDC hdc) {
		std::call_once(swapBuffersInit, OnSwapBufferInitialize);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Window");

		ImGui::End();

		ImGui::EndFrame();

		return swapBuffers(hdc);
	}
}

namespace Pointers {
	uintptr_t moduleBase;
	HMODULE hModule;
	Functions::swapBuffers swapBuffers;
}