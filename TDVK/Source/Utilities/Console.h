#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <source_location>
#include <chrono>
#include <format>

namespace Console {
	namespace Logger {
		auto static ToString(const std::source_location& source) -> std::string {
			return std::format("{}:{}:{}", std::filesystem::path(source.file_name()).filename().string(), source.function_name(), source.line());
		}

		auto static ToString(const auto& tp) -> std::string {
			return std::format("{:%F %T %Z}", tp);
		}

		auto static GetTime() -> std::string {
			std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			std::string s(10, '\0');
			std::strftime(&s[0], s.size(), "%H:%M:%S", std::localtime(&now));
			return s;
		}

		auto static Info(std::string_view const message) -> void { std::cout << std::format("[Info]  {} | {}", GetTime(), message) << '\n'; }
		auto static Warn(std::string_view const message) -> void { std::cout << std::format("[Warn]  {} | {}", GetTime(), message) << '\n'; }
		auto static Error(std::string_view const message) -> void { std::cout << std::format("[Error] {} | {}", GetTime(), message) << '\n'; }
		auto static Trace(std::string_view const message, std::source_location const source = std::source_location::current()) -> void {
			std::cout << std::format("[Trace] {} | {} | {}", GetTime(), ToString(source), message) << '\n';
		}
	}

	static FILE* console;

	static void Initialize() {
		AllocConsole();

		freopen_s(&console, "CONOUT$", "w", stdout);
		freopen_s(&console, "STDERR$", "w", stderr);
		freopen_s(&console, "STDIN$", "r", stdin);
	}

	static void Close() { fclose(console); }
};