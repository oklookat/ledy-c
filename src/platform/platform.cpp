#include "platform.h"

void platform::sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

platform::AdvancedSleep::AdvancedSleep(int timeMs)
{
	sleepTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeMs);
}

void platform::AdvancedSleep::sleep()
{
	std::this_thread::sleep_until(sleepTime);
}
