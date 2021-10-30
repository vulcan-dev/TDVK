#pragma once

#include "Utilities/Console.h"

#define CORE_INFO(str)		::Console::Logger::Info(str)
#define CORE_WARN(str)		::Console::Logger::Warn(str)
#define CORE_ERROR(str)		::Console::Logger::Error(str)
#define CORE_TRACE(str)		::Console::Logger::Trace(str)