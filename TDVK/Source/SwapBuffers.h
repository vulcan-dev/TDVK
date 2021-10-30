#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>
#include <vector>
#include <functional>

void InitializeHIDHook();
void DestroyHIDHook();
void InitializeSwapBufferHook();
void DestroySwapBufferHook();