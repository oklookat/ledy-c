#include "effect.h"

using namespace effect;

static float getFreqPercents(dsp::ChannelFFT fft, double sampleRate, unsigned short fromHz, unsigned short toHz)
{
	if (fft.size() == 0)
		return 0;

	auto idxs = dsp::getFrequencyRangeIndexes(fft, sampleRate, fromHz, toHz);
	auto fftCut = dsp::ChannelFFT(fft.begin() + idxs.first, fft.begin() + idxs.second + 1);
	auto amp = dsp::amplitudeMulti(fftCut);

	auto volume = (amp / MAX_AMPLITUDE) * 100.0;
	if (volume > 100)
		volume = 100;
	else if (volume < 0)
		volume = 0;

	return volume;
};

static std::vector<float> frequencyRange(float start, float end, unsigned short numBands)
{
	std::vector<float> freqs(numBands);
	auto delta = (end - start) / float(numBands);
	for (size_t i = 0; i < numBands; i++)
	{
		freqs[i] = start + float(i) * delta;
	}
	return freqs;
}

SpectrumRange::SpectrumRange(
	double sampleRate,
	unsigned short fromHz,
	unsigned short toHz,
	float alphaDecay, float alphaRise)
{
	this->sampleRate = sampleRate;
	this->fromHz = fromHz;
	this->toHz = toHz;
	filter = std::shared_ptr<dsp::ExponentialSmoothing>(
		new dsp::ExponentialSmoothing(percents, alphaDecay));
}

void SpectrumRange::process(const dsp::ChannelFFT &spectrum)
{
	auto idxs = dsp::getFrequencyRangeIndexes(spectrum, sampleRate, fromHz, toHz);
	this->index1 = idxs.first;
	this->index2 = idxs.second;

	percentsDirty = getFreqPercents(spectrum, sampleRate, fromHz, toHz);
	percents = filter->update(percentsDirty);
	delta = std::abs(percentsDirty - float(prevPercents));
	prevPercents = percentsDirty;
}

void Spectrum::processSamples(const platform::audio::Samples &samples)
{
	auto sampleRate = this->sampleRate;
	this->mono = samples.mono;
	this->monoFFT = dsp::makeFFT(samples.mono);

	auto fftSize = monoFFT.size();
	for (size_t i = 0; i < fftSize; i++)
	{
		auto amp = dsp::amplitudeSingle(monoFFT[i]);
		if (amp > maxAmplitude)
		{
			maxAmplitude = amp;
		};
	}
}

void Spectrum::visualize(const platform::audio::Samples samples, client::LEDS &leds)
{
	processSamples(samples);
	auto rangesSize = ranges.size();
	auto rangeSize = std::round(leds.size() / ranges.size());
	for (size_t i = 0; i < rangesSize; i++)
	{
		ranges[i].process(monoFFT);
		auto startIndex = i * rangeSize;
		auto endIndex = (i + 1) * rangeSize;
		for (size_t x = startIndex; x < endIndex; x++)
		{
			leds[x].set(255, 0, 0);
			leds[x].setBrightness(ranges[i].percents);
		}
	}
}

Spectrum::Spectrum(
	double sampleRate,
	float alphaDecay, float alphaRise,
	float fromHz, float toHz,
	unsigned short numBands)
{
	this->sampleRate = sampleRate;

	maxAmplitude = 0;
	mono = {};

	auto freqs = frequencyRange(fromHz, toHz, numBands);
	auto freqsSize = freqs.size();
	ranges.reserve(freqsSize);

	for (size_t i = 0; i < freqsSize; i++)
	{
		auto fFromHz = freqs[i];
		auto fToHz = 0.0;
		if (i + 1 >= freqsSize)
			fToHz = fFromHz;
		else
			fToHz = freqs[i + 1];
		ranges.push_back(SpectrumRange(sampleRate, fFromHz, fToHz, alphaDecay, alphaRise));
	}
}
