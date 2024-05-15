#pragma once

#include <iostream>
#include <string>
#include <array>

#include "color.h"
#include "finder.h"

// thread
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

// ixwebsocket
#include "boost/asio.hpp"			   // ip + ixwebsocket dep.
#include "boost/system/error_code.hpp" // ip + ixwebsocket dep.
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

namespace client
{
	constexpr unsigned short TARGET_WS_PORT = 80;

	constexpr unsigned short LEDS_COUNT = 260;
	typedef std::array<color::TheRGB, LEDS_COUNT> LEDS;

	typedef unsigned char _Command;

	typedef enum : _Command
	{
		CommandSetColors,
	} Command;

	class Client
	{
	public:
		Client();
		~Client();
		void connect();
		void visualize(LEDS &leds);

	private:
		ix::WebSocket conn;
		//
		bool stopFlag = false;
		bool disabled = false;
		std::thread websocketThread;
		std::queue<LEDS> ledsQueue;
		std::mutex queueMutex;
		std::condition_variable queueCondition;
		void setColors(LEDS &leds);
		void websocketLoop();
	};
};
