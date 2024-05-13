#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#define PLATFORM_WINDOWS
#define PLATFORM_SUPPORTED 1
#elif defined(unix) || defined(__linux__) || defined(BSD) || defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_UNIX
#endif

namespace platform
{
	void initialize(void);
	void sleep(int ms);
}
