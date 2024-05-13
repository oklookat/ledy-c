#pragma once

#include "../platform/platform.h"

// ip
#include "boost/asio.hpp"

// #include <format>
#include <exception>
#include "windows.h"
#include "windns.h"

namespace finder
{
	constexpr auto LEDY_TARGET_QUERY = L"_ledy._tcp.local";
	constexpr auto LEDY_TARGET_SERVER = "ledy.local";

	typedef boost::asio::ip::address_v4 IPv4;
	IPv4 findServer();
}
