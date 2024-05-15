#include "debug.h"

using namespace effect;

debug::Blink::Blink(int everyMs)
{
	this->everyMs = everyMs;
}

void debug::Blink::visualize(client::LEDS &leds)
{
	auto sleeper = platform::AdvancedSleep(everyMs);
	auto ledsCount = leds.size();
	auto color = disabled ? 255 : 0;
	for (size_t i = 0; i < ledsCount; i++)
	{
		leds[i].R = color;
		leds[i].G = color;
		leds[i].B = color;
	}
	disabled = !disabled;
	sleeper.sleep();
}
