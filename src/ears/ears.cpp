#include "ears.h"

using namespace ears;

static double _sampleRate = 44100.0;
static double _latency = 0.0;
static int _channels = 2;

static void mergeChannels(const dsp::Channels &channels, dsp::Channel &monoOut)
{
	size_t maxLength = 0;
	auto numChannels = channels.size();

	// Find the maximum length among all channels.
	for (size_t i = 0; i < numChannels; i++)
	{
		if (channels[i].size() > maxLength)
		{
			maxLength = channels[i].size();
		}
	}

	// Sum up all channel samples into monoOut.
	for (size_t x = 0; x < numChannels; x++)
	{
		for (size_t y = 0; y < channels[x].size(); y++)
		{
			monoOut[y] += channels[x][y];
		}
	}

	// Calculate the average by dividing by the number of channels.
	for (size_t i = 0; i < maxLength; i++)
	{
		monoOut[i] /= float(numChannels);
	}
}

static int PaStreamRecord(const void *input, void *output,
						  unsigned long frameCount,
						  const PaStreamCallbackTimeInfo *timeInfo,
						  PaStreamCallbackFlags statusFlags, void *userData)
{
	if (input == nullptr || userData == nullptr)
	{
		return paContinue;
	}

	ears::Samples result{};
	result.sampleRate = _sampleRate;
	result.channels = _channels;
	result.framesPerBuffer = dsp::BUFFER_SIZE;

	const Sample *in = (const ears::Sample *)input;

	// Samples.
	dsp::Channels channels(_channels, dsp::Channel{0.0});
	for (size_t x = 0; x < frameCount; x++)
	{
		for (size_t y = 0; y < _channels; y++)
		{
			channels[y][x] = *in++;
		}
	}

	// Set pcm.
	mergeChannels(channels, result.mono);

	// Callback.
	SamplesCallback cb = (SamplesCallback)userData;
	cb(result);
	return paContinue;
}

static PaError checkError(PaError err)
{
	if (err > -1)
	{
		return err;
	}
	if (err != paNoError)
	{
		auto errMsg = std::format("PortAudio ({}): {}", err, Pa_GetErrorText(err));
		throw std::runtime_error(errMsg);
	}
	return err;
}

Listener::Listener(SamplesCallback cb)
{
	this->samplesCb = cb;

	// Init.
	checkError(Pa_Initialize());

	// Select host API.
	auto hostApisCount = Pa_GetHostApiCount();
	checkError(hostApisCount);
	const PaHostApiInfo *api = nullptr;
	for (auto i = 0; i < hostApisCount - 1; i++)
	{
		auto tapi = Pa_GetHostApiInfo(i);
		if (tapi->type == paWASAPI)
		{
			api = tapi;
			break;
		}
	}
	if (api == nullptr)
	{
		throw std::runtime_error("no available host apis");
	}

	// Select device.
	// Get total count.
	auto deviceCount = Pa_GetDeviceCount();
	checkError(deviceCount);

	// Get default out (speakers).
	auto defaultOutIdx = Pa_GetDefaultOutputDevice();
	checkError(defaultOutIdx);
	if (defaultOutIdx == paNoDevice)
	{
		throw std::runtime_error("default audio device not found");
	}
	auto defaultOut = Pa_GetDeviceInfo(defaultOutIdx);

	// Find loopback device by output devuce.
	for (auto i = 0; i < deviceCount - 1; i++)
	{
		auto di = Pa_GetDeviceInfo(i);
		if (defaultOut->maxOutputChannels != di->maxInputChannels)
		{
			continue;
		}
		if (di->maxOutputChannels != 0)
		{
			continue;
		}
		if (std::string(di->name).starts_with(defaultOut->name))
		{
			this->device = di;
			this->deviceIndex = i;
			break;
		}
	}

	if (this->device == nullptr)
	{
		throw std::runtime_error("no available audio devices");
	}

	_sampleRate = this->device->defaultSampleRate;
	this->sampleRate = _sampleRate;
	_latency = this->device->defaultLowInputLatency;
	_channels = this->device->maxInputChannels;
}

Listener::~Listener()
{
	checkError(Pa_Terminate());
}

void Listener::listen()
{
	PaStreamParameters inputParameters{};
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.channelCount = _channels;
	inputParameters.device = this->deviceIndex;
	inputParameters.suggestedLatency = _latency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	PaStream *stream = NULL;
	auto err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,
		_sampleRate,
		dsp::BUFFER_SIZE,
		paNoFlag,
		PaStreamRecord,
		this->samplesCb);

	checkError(err);
	checkError(Pa_StartStream(stream));

	return;
}
