#pragma once

namespace color
{
	class TheRGB
	{
	public:
		unsigned char R;
		unsigned char G;
		unsigned char B;
		TheRGB()
		{
			set(0, 0, 0);
		};
		TheRGB(unsigned char r, unsigned char g, unsigned char b)
		{
			set(r, g, b);
		};
		void set(unsigned char r, unsigned char g, unsigned char b)
		{
			R = r;
			G = g;
			B = b;
		}
		void setBrightness(float percents)
		{
			percents > 100.0 ? percents = 0 : percents < 0 ? percents = 0
														   : percents = percents;
			auto factor = percents / 100.0;
			R = R * factor;
			G = G * factor;
			B = B * factor;
		};
	};
}
