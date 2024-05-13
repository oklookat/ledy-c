#include "finder.h"

using namespace finder;

#if defined(PLATFORM_WINDOWS)

typedef struct onIpFinder
{
	bool found = false;
	IPv4 ip;
} onIpFinder;

// Just std::string(str) not work. So...
static std::string pStrToString(const PSTR str)
{
	const int maxDnsNameLen = 253;
	size_t strLen = 0;
	// Allocate buffer on stack with space for null terminator.
	char resBuff[maxDnsNameLen + 1];
	size_t buffI = 0;

	for (size_t i = 0; i < maxDnsNameLen; i++)
	{
		auto sChar = str[i];
		// Check for end of string.
		if (sChar == '\0' && i > 0 && str[i - 1] == '\0')
		{
			// Ensure null termination.
			resBuff[buffI] = '\0';
			break;
		}
		if (sChar == '\0')
		{
			continue;
		}
		resBuff[buffI++] = sChar;
		strLen++;
	}

	return std::string(resBuff, strLen);
}

static void completionRoutine(PVOID pQueryContext, PDNS_QUERY_RESULT pQueryResults)
{
	auto finder = static_cast<onIpFinder *>(pQueryContext);
	finder->found = false;

	if (pQueryResults->pQueryRecords == NULL)
	{
		return;
	}

	auto found = false;
	auto firstRecord = pQueryResults->pQueryRecords;
	auto records = pQueryResults->pQueryRecords;

	while (records != nullptr)
	{
		if (records->wType != DNS_TYPE_A)
		{
			records = records->pNext;
			continue;
		}

		auto record = records->Data;
		auto named = pStrToString(records->pName);
		if (named != LEDY_TARGET_SERVER)
		{
			records = records->pNext;
			continue;
		}

		found = true;
		finder->ip = boost::asio::ip::make_address_v4(ntohl(record.A.IpAddress));
		break;
	}

	DnsRecordListFree(firstRecord, records);
	finder->found = found;
}

IPv4 finder::findServer()
{
	DNS_SERVICE_CANCEL cancel{};

	DNS_SERVICE_BROWSE_REQUEST req{};
	req.Version = DNS_QUERY_REQUEST_VERSION2;
	req.InterfaceIndex = 0;
	req.QueryName = LEDY_TARGET_QUERY;
	req.pBrowseCallbackV2 = completionRoutine;
	onIpFinder onFinder;
	req.pQueryContext = &onFinder;

	if (DnsServiceBrowse(&req, &cancel) != DNS_REQUEST_PENDING)
	{
		throw std::runtime_error(std::format("dns error: {}", GetLastError()));
	}

	while (!onFinder.found)
	{
		platform::sleep(1);
	}

	DnsServiceBrowseCancel(&cancel);

	return onFinder.ip;
}

#endif
