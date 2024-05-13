#include <windows.h>

#include "platform.h"

void platform::initialize()
{
	SetConsoleOutputCP(CP_UTF8);
}

void platform::sleep(int ms)
{
	Sleep(ms);
}
