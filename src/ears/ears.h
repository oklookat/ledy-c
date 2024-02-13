#pragma once

#include <stdio.h>
#include <complex>
#include <array>
#include <vector>

#include "portaudio/include/portaudio.h"

#include "../dsp/dsp.h"

#define PA_SAMPLE_TYPE paFloat32

namespace ears
{
	typedef float Sample;


	typedef struct Samples
	{
		double sampleRate;
		unsigned short framesPerBuffer;
		unsigned char channels;

		dsp::Channel mono;
	} Samples;

	typedef void (*SamplesCallback)(const Samples &samples);

	class Listener
	{
	public:
		Listener(SamplesCallback cb);
		~Listener();
		void listen();
		double sampleRate;
	private:
		SamplesCallback samplesCb = nullptr;
		PaDeviceIndex deviceIndex = -1;
		const PaDeviceInfo *device = nullptr;
	};
}
