#include "finder.h"

using namespace finder;

#if PLATFORM_NAME == PLATFORM_NAME_WINDOWS

typedef struct onIpFinder {
	bool found = false;
	IPv4 ip;
} onIpFinder;

// Omg.
static std::string pStrToString(const PSTR str) {
	// I tried to convert wchar to char, but it dont work. So...
	const int maxDnsNameLen = 253;
	size_t strLen = 0;
	char* resBuff = new char[maxDnsNameLen];
	size_t buffI = 0;
	for (size_t i = 0; i < maxDnsNameLen; i++)
	{
		auto sChar = str[i];
		if (sChar == NULL && i > 0 && str[i - 1] == NULL) {
			resBuff[buffI + 1] = NULL;
			break;
		}
		if (sChar == NULL) {
			continue;
		}
		resBuff[buffI] = sChar;
		buffI++;
		strLen++;
	}
	auto result = std::string(resBuff, strLen);
	delete[]resBuff;
	return result;
}

static void completionRoutine(PVOID pQueryContext, PDNS_QUERY_RESULT pQueryResults) {
	onIpFinder* finder = static_cast<onIpFinder*>(pQueryContext);
	if (pQueryResults->pQueryRecords == NULL) {
		return;
	}

	auto found = false;
	auto firstRecord = pQueryResults->pQueryRecords;
	auto records = pQueryResults->pQueryRecords;
	while (records != nullptr) {
		if (records->wType == DNS_TYPE_A) {
			auto record = records->Data;
			finder->ip = boost::asio::ip::make_address_v4(ntohl(record.A.IpAddress));
			auto named = pStrToString(records->pName);
			//std::print("found ip: {} | name: {}\n", finder->ip.to_string().c_str(), named);
			if (named == LEDY_TARGET_SERVER) {
				found = true;
				break;
			}
		}
		records = records->pNext;
	}
	DnsRecordListFree(firstRecord);
	finder->found = true;
}

IPv4 finder::findServer()
{
	onIpFinder onFinder;
	onFinder.found = false;

	DNS_SERVICE_CANCEL cancel{};

	DNS_SERVICE_BROWSE_REQUEST req{};
	req.Version = DNS_QUERY_REQUEST_VERSION2;
	req.InterfaceIndex = 0;
	req.QueryName = LEDY_TARGET_QUERY;
	req.pBrowseCallbackV2 = completionRoutine;
	req.pQueryContext = &onFinder;

	if (DnsServiceBrowse(&req, &cancel) != DNS_REQUEST_PENDING) {
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
