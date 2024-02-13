#pragma once

#include <complex>
#include <vector>
#include <array>
#include <utility>

#include "kiss_fft.h"

namespace dsp
{
	constexpr unsigned short BUFFER_SIZE = 1024;
	constexpr unsigned short BUFFER_OUT_FFT_SIZE = BUFFER_SIZE;
	typedef std::complex<float> FFTSingle;
	typedef std::array<float, BUFFER_SIZE> Channel;
	typedef std::vector<Channel> Channels;
	typedef std::vector<FFTSingle> ChannelFFT;

	class ExpFilter
	{
	public:
		ExpFilter(float value, float alphaDecay, float alphaRise);
		float update(float newValue);

	private:
		float value;
		float alphaDecay;
		float alphaRise;
	};

	ChannelFFT makeFFT(const Channel& chan);

	float amplitudeMulti(const ChannelFFT& spectrum);

	float amplitudeSingle(const FFTSingle& fft);

	std::pair<int, int> getFrequencyRangeIndexes(
		const ChannelFFT& spectrum,
		double sampleRate,
		unsigned short fromHz,
		unsigned short toHz);
}
