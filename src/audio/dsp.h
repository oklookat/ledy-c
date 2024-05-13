#pragma once

#include <complex>
#include <vector>
#include <array>
#include <utility>

#include "kiss_fft.h"

#include "../platform/audio.h"

namespace dsp
{
	typedef std::complex<float> FFTSingle;

	typedef std::vector<FFTSingle> ChannelFFT;

	class ExponentialSmoothing
	{
	public:
		ExponentialSmoothing(float value, float decay);
		float update(float newValue);

	private:
		float smoothedValue;
		float alpha;
	};

	ChannelFFT makeFFT(const platform::audio::Channel &chan);

	float amplitudeMulti(const ChannelFFT &spectrum);

	float amplitudeSingle(const FFTSingle &fft);

	std::pair<int, int> getFrequencyRangeIndexes(
		const ChannelFFT &spectrum,
		double sampleRate,
		unsigned short fromHz,
		unsigned short toHz);
}
