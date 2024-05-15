#include <windows.h>

#include "platform.h"

void platform::initialize()
{
	SetConsoleOutputCP(CP_UTF8);
}
