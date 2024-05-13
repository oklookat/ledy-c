#pragma once

#include <vector>
#include <string>

#include "platform.h"

namespace platform
{
	namespace audio
	{
		class Exception : public std::exception
		{
		public:
			explicit Exception(const char *message)
				: msg_(message) {}

			explicit Exception(const std::string &message)
				: msg_(message) {}

			explicit Exception(const std::string &message, long code)
				: msg_(message), code_(code) {}

			virtual ~Exception() noexcept {}

			virtual const char *what() const noexcept
			{
				return msg_.c_str();
			}

		protected:
			std::string msg_;
			long code_;
		};

		typedef std::vector<float> Channel;

		typedef struct DeviceInfo
		{
			double sampleRate;
		} DeviceInfo;

		typedef struct Samples
		{
			unsigned short framesPerBuffer;
			Channel mono;
		} Samples;

		typedef bool (*SamplesCallback)(const Samples samples);

		class Sink
		{
		public:
			DeviceInfo deviceInfo;
			SamplesCallback samplesCb;
			// After creating, DeviceInfo will be available.
			// expectedLatencyMs cannot be lower 5.
			Sink(SamplesCallback cb, int expectedLatencyMs = 25);
			~Sink();
			// Start listening audio, call SamplesCallback every ~expectedLatency(?). Blocks current thread.
			void listen();

		private:
			// Platform-specific data.
			void *pCtx = nullptr;
			int expectedLatencyMs = 25;
		};
	};
}
