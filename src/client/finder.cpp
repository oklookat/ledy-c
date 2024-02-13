#include "finder.h"

using namespace finder;

inline static IPv4 getIpByHostname(std::string hostname) {
	boost::asio::io_context ioContext;
	boost::system::error_code ec;

	boost::asio::ip::tcp::resolver resolver(ioContext);
	boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(hostname, "", ec);

	if (ec)
	{
		auto errMsg = std::format("platform::getIpByHostname / boost::system::error_code: {}", ec.message());
		throw std::runtime_error(errMsg);
	}

	return endpoint->endpoint().address().to_v4();
}

static void dnsCallback(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex,
	DNSServiceErrorType errorCode, const char* serviceName, const char* regtype,
	const char* replyDomain, void* context)
{
	auto onFinder = static_cast<onIpFinder*>(context);

	if (errorCode != kDNSServiceErr_NoError)
	{
		auto errMsg = std::format("dnsCb: {}", errorCode);
		throw std::runtime_error(errMsg);
	}

	if (flags & kDNSServiceFlagsAdd)
	{
		if (strcmp(serviceName, TARGET_HOSTNAME) != 0)
		{
			return;
		}
		onFinder->ip = getIpByHostname(std::string(serviceName));
		onFinder->found = true;
	}
};

IPv4 finder::findServer()
{
	onIpFinder onFinder;
	onFinder.found = false;

	DNSServiceRef serviceRef;

	DNSServiceErrorType err = DNSServiceBrowse(&serviceRef, 0, 0,
		TARGET_SERVICE, TARGET_DOMAIN, dnsCallback, &onFinder);
	if (err != kDNSServiceErr_NoError)
	{
		DNSServiceRefDeallocate(serviceRef);
		auto errMsg = std::format("Client::Client / DNSServiceBrowse: {}", err);
		if (err == kDNSServiceErr_ServiceNotRunning)
		{
			errMsg = std::string("Bonjour not started or not installed");
		}
		throw std::runtime_error(errMsg);
	}

	while (!onFinder.found)
	{
		printf("trying to find the ip...\n");
		err = DNSServiceProcessResult(serviceRef);
		if (err != kDNSServiceErr_NoError)
		{
			DNSServiceRefDeallocate(serviceRef);
			auto errMsg = std::format("Client::Client / DNSServiceProcessResult: {}", err);
			throw std::runtime_error(errMsg);
		}
		platform::sleep(10);
	}

	DNSServiceRefDeallocate(serviceRef);

	return onFinder.ip;
}
