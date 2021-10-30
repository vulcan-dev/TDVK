#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "Logger.h"

namespace Utilities {
	namespace Console {
		static FILE* console;
		static Logger logger;

		static void Initialize() {
			AllocConsole();

			freopen_s(&console, "CONOUT$", "w", stdout);
			freopen_s(&console, "STDERR$", "w", stderr);
			freopen_s(&console, "STDIN$", "r", stdin);

			Logger::Initialize();
		}

		static void Close() { fclose(console); }
	}
};