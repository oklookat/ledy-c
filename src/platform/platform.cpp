#include "platform.h"

void platform::sleep(int ms)
{
#if PLATFORM_NAME == PLATFORM_WINDOWS
	Sleep(ms);
#elif PLATFORM_NAME == PLATFORM_UNIX
	// (microseconds).
	usleep(1000 * ms);
#endif
}

void platform::setupConsole()
{
#if PLATFORM_NAME == PLATFORM_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
#endif
}
