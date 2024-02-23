#include "main.h"

client::LEDS leds;
client::Client cl;
effect::Spectrum *eff;

static void cb(const ears::Samples &samples)
{
	eff->visualize(samples, leds);
	cl.setColors(leds);
}

int main()
{
	platform::setupConsole();
	auto listener = ears::Listener(cb);

	cl.connect();
	printf("connected\n");

	cl.setColorCorrection(color::ColorCorrectionTypicalLEDStrip);
	cl.setColorTemperature(color::ColorCorrectionUncorrectedColor);

	auto spec = effect::Spectrum(listener.sampleRate, 0.3, 0.3, 30, 1000, 14);
	eff = &spec;

	listener.listen();

	while (1)
	{
		platform::sleep(50000);
	}

	return 0;
}
