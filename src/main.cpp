#include "main.h"

#include "audio.h"

client::LEDS leds;
client::Client cl;
effect::Spectrum *eff;

// TODO: move every os-thing to platform dir. Rename audio dir. And so on.

bool cb(const platform::audio::Samples samples)
{
	eff->visualize(samples, leds);
	cl.setColors(leds);
	return true;
}

int main()
{
	platform::initialize();

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

	auto spec = effect::Spectrum(sink->deviceInfo.sampleRate, 0.2, 0.2, 20, 3000, 24);
	eff = &spec;

	std::thread captureThread([sink]()
							  {
    try {
        sink->listen();
    } catch (const platform::audio::Exception& e) {
		delete sink;
        std::cerr << "Audio capture error: " << e.what() << std::endl;
    } });

	cl.connect();
	printf("Connected.\n");
	captureThread.join();

	while (1)
	{
		platform::sleep(50000);
	}

	return 0;
}
