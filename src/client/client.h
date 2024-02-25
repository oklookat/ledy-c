#pragma once

#include <iostream>
#include <string>
#include <array>

#include "color.h"
#include "finder.h"

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
	typedef std::array<color::RGB, LEDS_COUNT> LEDS;

	typedef unsigned char tCommand;

	typedef enum : tCommand
	{
		CommandSetColors,
		CommandSetColorCorrection,
		CommandSetColorTemperature
	} Command;

	class Client
	{
	public:
		Client();
		~Client();
		void connect();
		void setColors(LEDS& leds);
		void setColorCorrection(color::tCorrection v);
		void setColorTemperature(color::tTemperature v);

	private:
		ix::WebSocket conn;
	};
};
