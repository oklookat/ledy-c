#include "dsp.h"

using namespace dsp;

ExponentialSmoothing::ExponentialSmoothing(float value, float decay)
{
	smoothedValue = value;
	alpha = decay;
}

float ExponentialSmoothing::update(float newValue)
{
	smoothedValue = alpha * newValue + (1 - alpha) * smoothedValue;
	return smoothedValue;
}

ChannelFFT dsp::makeFFT(const platform::audio::Channel &chan)
{
	auto chanSize = chan.size();

	// Setup.
	kiss_fft_cfg cfg = kiss_fft_alloc(chanSize, 0, NULL, NULL);
	auto in = new kiss_fft_cpx[chanSize];

	// Fill data.
	for (size_t i = 0; i < chanSize; ++i)
	{
		in[i].r = chan[i];
		in[i].i = 0.0;
	}

	// Run & release.
	auto output = new kiss_fft_cpx[chanSize];
	kiss_fft(cfg, in, output);
	free(cfg);
	delete[] in;

	// Convert output.
	auto result = ChannelFFT(chanSize);
	for (size_t i = 0; i < chanSize; i++)
	{
		result[i] = std::complex<float>(output[i].r, output[i].i);
	}
	delete[] output;

	return result;
}

float dsp::amplitudeMulti(const ChannelFFT &spectrum)
{
	float sum = 0.0;
	auto specSize = spectrum.size();
	for (size_t i = 0; i < specSize; i++)
	{
		sum += amplitudeSingle(spectrum[i]);
	}
	return sum;
}

float dsp::amplitudeSingle(const FFTSingle &fft)
{
	return std::abs(fft);
}

std::pair<int, int> dsp::getFrequencyRangeIndexes(
	const ChannelFFT &spectrum,
	double sampleRate,
	unsigned short fromHz,
	unsigned short toHz)
{
	auto specSize = spectrum.size();
	double k = sampleRate / double(specSize);

	auto fromIndex = int(std::round(double(fromHz) / k));
	auto toIndex = int(std::round(double(toHz) / k));
	if (fromIndex < 0)
		fromIndex = 0;
	if (toIndex >= specSize)
		toIndex = specSize - 1;

	return std::pair<int, int>(fromIndex, toIndex);
}
