#include "audio.h"

#include <windows.h>
#include <audioclient.h>
#include <mmdeviceapi.h>

using namespace platform;

class PlatformContext
{
public:
	HRESULT hr = 0;
	IMMDeviceEnumerator *pEnumerator = nullptr;
	IMMDevice *pDevice = nullptr;
	IAudioClient *pAudioClient = nullptr;
	WAVEFORMATEX *pWaveFormat = nullptr;
	UINT32 bufferFrameCount = 0;
	IAudioCaptureClient *pCaptureClient = nullptr;
	unsigned long optimalBuff;
	unsigned long maxAllowedBuff;

	~PlatformContext()
	{
		if (pEnumerator != nullptr)
		{
			pEnumerator->Release();
		}
		if (pDevice != nullptr)
		{
			pDevice->Release();
		}
		if (pAudioClient != nullptr)
		{
			pAudioClient->Stop();
			pAudioClient->Release();
		}
		if (pCaptureClient != nullptr)
		{
			pCaptureClient->Release();
		}
	}
	bool computeData(audio::SamplesCallback cb,
					 const BYTE *pData,
					 UINT32 numFramesAvailable, DWORD flags)
	{
		const int bytesPerSample = pWaveFormat->wBitsPerSample / 8;
		const int channels = pWaveFormat->nChannels;

		audio::Samples samples;
		samples.mono.resize(numFramesAvailable * channels);

		for (UINT32 i = 0; i < numFramesAvailable; ++i)
		{
			for (int channel = 0; channel < channels; ++channel)
			{
				const BYTE *pFrame = pData + (i * channels + channel) * bytesPerSample;
				float sampleValue = 0.0f;

				switch (bytesPerSample)
				{
				case 1:
				{
					// TODO: NOT TESTED.
					// 8-bit PCM
					sampleValue = static_cast<float>(*reinterpret_cast<const int8_t *>(pFrame)) / 128.0f;
					break;
				}
				case 2:
				{
					// TODO: NOT TESTED.
					// 16-bit PCM
					sampleValue = static_cast<float>(*reinterpret_cast<const int16_t *>(pFrame)) / 32768.0f;
					break;
				}
				case 3:
				{
					// TODO: NOT TESTED.
					// 24-bit PCM
					int32_t value24 = (static_cast<int32_t>(pFrame[2]) << 24) | (static_cast<int32_t>(pFrame[1]) << 16) | (static_cast<int32_t>(pFrame[0]) << 8);
					if (value24 & 0x800000)
						value24 |= ~0xFFFFFF;
					sampleValue = static_cast<float>(value24) / 8388608.0f;
					break;
				}
				case 4:
				{
					// 32-bit PCM
					sampleValue = *reinterpret_cast<const float *>(pFrame);
					// Sound have low volume.
					// TODO: maybe amplify it not here?
					sampleValue *= (bytesPerSample / 2);
					break;
				}
				default:
					break;
				}

				samples.mono[i * channels + channel] = sampleValue;
			}
		}

		samples.framesPerBuffer = numFramesAvailable;

		if (numFramesAvailable > 0 && !samples.mono.empty())
		{
			return cb(samples);
		}

		return true;
	}
};

audio::Sink::Sink(SamplesCallback cb, int expectedLatencyMs)
{
	if (expectedLatencyMs < 5)
	{
		throw Exception("expectedLatencyMs", expectedLatencyMs);
	}

	samplesCb = cb;
	this->expectedLatencyMs = expectedLatencyMs;

	pCtx = new PlatformContext;
	auto ptCtx = (PlatformContext *)pCtx;

	// Initialize COM.
	HRESULT hr;
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		throw Exception("CoInitializeEx", hr);
	}

	// Create a device enumerator.
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		NULL,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(void **)&ptCtx->pEnumerator);
	if (FAILED(hr))
	{
		throw Exception("CoCreateInstance", hr);
	}

	// Get the default audio endpoint device.
	hr = ptCtx->pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &ptCtx->pDevice);
	ptCtx->pEnumerator->Release();
	if (FAILED(hr))
	{
		throw Exception("pEnumerator->GetDefaultAudioEndpoint", hr);
	}

	// Activate the audio client.
	hr = ptCtx->pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **)&ptCtx->pAudioClient);
	ptCtx->pDevice->Release();
	if (FAILED(hr))
	{
		throw Exception("pDevice->Activate", hr);
	}

	// Get the mix format.
	hr = ptCtx->pAudioClient->GetMixFormat(&ptCtx->pWaveFormat);
	if (FAILED(hr))
	{
		throw Exception("pAudioClient->GetMixFormat", hr);
	}
	deviceInfo.sampleRate = ptCtx->pWaveFormat->nSamplesPerSec;

	// expectedLatencyMs * nanoseconds things, idk.
	REFERENCE_TIME hnsRequestedDuration = expectedLatencyMs * 10000;

	// Initialize the audio client in shared mode with loopback.
	hr = ptCtx->pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
										 AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, ptCtx->pWaveFormat, NULL);
	if (FAILED(hr))
	{
		throw Exception("pAudioClient->Initialize", hr);
	}

	// Get the buffer size.
	hr = ptCtx->pAudioClient->GetBufferSize(&ptCtx->bufferFrameCount);
	if (FAILED(hr))
	{
		throw Exception("pAudioClient->GetBufferSize", hr);
	}
	// TODO: check formulas on another audio devices.
	ptCtx->optimalBuff = (ptCtx->pWaveFormat->nSamplesPerSec * (expectedLatencyMs / 5)) / (1000 * ptCtx->pWaveFormat->nChannels);
	ptCtx->maxAllowedBuff = (ptCtx->pWaveFormat->nSamplesPerSec * (expectedLatencyMs / 2)) / (1000 * ptCtx->pWaveFormat->nChannels);

	// Get the capture client.
	hr = ptCtx->pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void **)&ptCtx->pCaptureClient);
	if (FAILED(hr))
	{
		throw Exception("pAudioClient->GetService", hr);
	}
}

audio::Sink::~Sink()
{
	auto ptCtx = static_cast<PlatformContext *>(pCtx);
	delete ptCtx;
	CoUninitialize();
}

void audio::Sink::listen()
{
	auto ptCtx = (PlatformContext *)pCtx;

	// Start capturing.
	auto hr = ptCtx->pAudioClient->Start();
	if (FAILED(hr))
	{
		throw Exception("pAudioClient->Start", hr);
	}

	// Main capture loop.
	bool ready = false;
	while (true)
	{
		auto sleeper = platform::AdvancedSleep(1);
		BYTE *pData;
		UINT32 numFramesAvailable;
		DWORD flags;
		hr = ptCtx->pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
		if (FAILED(hr))
		{
			if (hr == AUDCLNT_S_BUFFER_EMPTY)
			{
				continue;
			}
			if (hr == AUDCLNT_E_DEVICE_INVALIDATED)
			{
				// Device disabled.
				break;
			}
			throw Exception("pCaptureClient->GetBuffer", hr);
		}

		// Wait for optimal buffer.
		// Without this, there will either be delays/lags or insufficient data for FFT.
		auto bufferDiff = (ptCtx->bufferFrameCount / ptCtx->pWaveFormat->nChannels) - numFramesAvailable;
		if (bufferDiff >= ptCtx->optimalBuff && bufferDiff <= ptCtx->maxAllowedBuff)
		{
			// Compute data.
			// TODO: move to another thread? For now its not lagging. So, idk.
			if (!ptCtx->computeData(samplesCb, pData, numFramesAvailable, flags))
			{
				break;
			}
		}

		// Release the buffer.
		hr = ptCtx->pCaptureClient->ReleaseBuffer(numFramesAvailable);
		if (FAILED(hr))
		{
			throw Exception("pCaptureClient->ReleaseBuffer", hr);
		}
		sleeper.sleep();
	}
}
