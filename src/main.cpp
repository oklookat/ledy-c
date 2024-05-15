#include "main.h"

client::LEDS leds;
client::Client cl;
effect::Spectrum *eff;

auto debugBlink = effect::debug::Blink(100);

// TODO: Add Razer Chroma support.

bool cb(const platform::audio::Samples samples)
{
	eff->visualize(samples, leds);
	cl.visualize(leds);
	return true;
}

static void debugVisualize()
{
	while (1)
	{
		debugBlink.visualize(leds);
		cl.visualize(leds);
	}
}

int main()
{
	platform::initialize();

	cl.connect();
	printf("Connected.\n");
	// debugVisualize();

	platform::audio::Sink *sink;
	try
	{
		sink = new platform::audio::Sink(cb, 25);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Audio capture error: " << e.what() << std::endl;
		return 1;
	}
	// todo добавить другие эффекты, хз, спектр иногда скучный
	auto spec = effect::Spectrum(sink->deviceInfo.sampleRate, 0.2, 0.2, 20, 600, 30);
	eff = &spec;

	std::thread captureThread([sink]()
							  {
    try {
        sink->listen();
    } catch (const platform::audio::Exception& e) {
		delete sink;
        std::cerr << "Audio capture error: " << e.what() << std::endl;
    } });

	captureThread.join();

	while (1)
	{
		platform::sleep(50000);
	}

	return 0;
}
