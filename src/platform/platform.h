#pragma once

#define PLATFORM_WINDOWS 0
#define PLATFORM_UNIX 1

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define PLATFORM_NAME PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(unix) || defined(__linux__) || defined(BSD) || defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_NAME PLATFORM_UNIX
#include <unistd.h>
#endif

#include <string>
#include <iostream>

namespace platform
{
	void sleep(int ms);
	void setupConsole(void);
}
