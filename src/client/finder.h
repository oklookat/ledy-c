#pragma once

#include "../platform/platform.h"

// ip
#include "boost/asio.hpp"

// bonjour
#include "dns_sd.h"

namespace finder {
	constexpr char TARGET_DOMAIN[7] = "local.";
	constexpr char TARGET_SERVICE[11] = "_ledy._tcp";
	constexpr char TARGET_HOSTNAME[12] = "ledy";

	typedef boost::asio::ip::address_v4 IPv4;

	IPv4 findServer();

	typedef struct {
		bool found;
		IPv4 ip;
	} onIpFinder;
}
