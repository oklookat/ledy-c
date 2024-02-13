#pragma once

#include <memory>
#include <algorithm>
#include <iostream>

#include "../dsp/dsp.h"
#include "../client/client.h"
#include "../client/color.h"
#include "../ears/ears.h"

namespace effect
{
	constexpr float MAX_AMPLITUDE = 80.0;

	class SpectrumRange
	{
	public:
		SpectrumRange(
			double sampleRate,
			unsigned short fromHz,
			unsigned short toHz,
			float alphaDecay, float alphaRise);
		void process(const dsp::ChannelFFT& spectrum);
		double sampleRate = 0;
		size_t index1 = 0;
		size_t index2 = 0;
		float percents = 0;
		float prevPercents = 0;
		float percentsDirty = 0;
		float delta = 0;
		unsigned short fromHz = 0;
		unsigned short toHz = 0;

	private:
		std::shared_ptr<dsp::ExpFilter> filter = nullptr;
	};

	class Spectrum
	{
	public:
		Spectrum(double sampleRate, float alphaDecay, float alphaRise, float fromHz, float toHz, unsigned short numBands);
		void visualize(const ears::Samples& samples, client::LEDS& leds);

	private:
		void processSamples(const ears::Samples& samples);
		std::vector<SpectrumRange> ranges;
		double maxAmplitude;
		dsp::Channel mono;
		dsp::ChannelFFT monoFFT;
	};

};
