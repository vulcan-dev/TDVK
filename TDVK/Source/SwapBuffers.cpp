#include "SwapBuffers.h"
#include "Global.h"
#include "detours.h"
#include "Utilities/Logger.h"

std::once_flag swapBuffersInit;
WNDPROC oWndProc;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_INSERT) {
			Pointers::showMenu = !Pointers::showMenu;
			return true;
		}
		break;
	}

	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	if (Pointers::showMenu) return true;
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool hwCursor(int x, int y) {
	if (Pointers::showMenu) return false;

	return Pointers::ocursor(x, y);
}

void OnSwapBufferInitialize() {
	glewInit();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Pointers::gWindow);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.85f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

	style.Colors[ImGuiCol_Separator] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);

	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);

	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9f, 0.9f, 0.9f, 1.00f);
}

void InitializeHIDHook() {
	HMODULE u32 = GetModuleHandle(L"user32.dll");
	Pointers::ocursor = (Functions::hk_SetCursorPos)GetProcAddress(u32, "SetCursorPos");

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(Pointers::gWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Pointers::ocursor, hwCursor);
	DetourTransactionCommit();
}

void DestroyHIDHook() {
	SetWindowLongPtrA(Pointers::gWindow, GWLP_WNDPROC, LONG_PTR(oWndProc));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)Pointers::ocursor, hwCursor);
	DetourTransactionCommit();
}

bool hwglSwapBuffers(_In_ HDC hDc) {
	std::call_once(swapBuffersInit, OnSwapBufferInitialize);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiIO* IO = &ImGui::GetIO();
	IO->MouseDrawCursor = Pointers::showMenu;

	for (auto const& callback : Pointers::renderFunctions) {
		callback();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return Pointers::owglSwapBuffers(hDc);
}

void DestroySwapBufferHook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)Pointers::owglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}

void InitializeSwapBufferHook() {
	HMODULE OpenGL = GetModuleHandle(L"opengl32.dll");
	Pointers::owglSwapBuffers = (Functions::twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Pointers::owglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}