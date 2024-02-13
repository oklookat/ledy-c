#include "main.h"

client::LEDS leds;
client::Client cl;
effect::Spectrum *eff;


static void cb(const ears::Samples& samples)
{
	eff->visualize(samples, leds);
	cl.setColors(leds);
}

int main()
{
	platform::setupConsole();
	auto listener = ears::Listener(cb);

	cl.connect();
	cl.setColorCorrection(color::ColorCorrectionTypical8mmPixel);
	cl.setColorTemperature(color::ColorTemperatureStandardFluorescent);
	printf("connected\n");

	auto spec = effect::Spectrum(listener.sampleRate, 1, 1, 30, 1000, 14);
	eff = &spec;

	listener.listen();

	while (1)
	{
		platform::sleep(50000);
	}

	return 0;
}
