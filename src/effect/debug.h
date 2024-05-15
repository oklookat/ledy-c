#pragma once

#include "../client/client.h"
#include "../platform/platform.h"

namespace effect
{
	namespace debug
	{
		class Blink
		{
		public:
			Blink(int everyMs);
			void visualize(client::LEDS &leds);

		private:
			bool disabled = true;
			int everyMs;
		};
	}
}
