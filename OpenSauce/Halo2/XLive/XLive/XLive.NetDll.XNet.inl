/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

XLIVE_API INT WSAAPI NetDll_XNetStartup(__in_opt const XNetStartupParams * pxnsp) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetCleanup() {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetRandom(__out_bcount(cb) BYTE * pb, __in UINT cb)
{
	for (UINT x = 0; x < cb; x++) pb[x] = CAST(BYTE, rand());

	return NO_ERROR;
}

XLIVE_API INT WSAAPI NetDll_XNetCreateKey(__out XNKID * pxnkid, __out XNKEY * pxnkey) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetRegisterKey(__in const XNKID * pxnkid, __in const XNKEY * pxnkey) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetUnregisterKey(__in const XNKID * pxnkid) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetXnAddrToInAddr(__in const XNADDR * pxna, __in const XNKID * pxnkid, __out IN_ADDR * pina) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetServerToInAddr(__in const IN_ADDR ina, __in DWORD dwServiceId, __out IN_ADDR * pina) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetTsAddrToInAddr(__in const TSADDR * ptsa, __in DWORD dwServiceId, __in const XNKID * pxnkid, __out IN_ADDR * pina) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetInAddrToXnAddr(__in const IN_ADDR ina, __out_opt XNADDR * pxna, __out_opt XNKID * pxnkid) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetInAddrToServer(__in const IN_ADDR ina, __out IN_ADDR *pina) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetInAddrToString(__in const IN_ADDR ina, __out_ecount(cchBuf) char * pchBuf, __in INT cchBuf) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetUnregisterInAddr(__in const IN_ADDR ina) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetXnAddrToMachineId(__in const XNADDR * pxnaddr, __out ULONGLONG * pqwMachineId) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetConnect(__in const IN_ADDR ina) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API DWORD WSAAPI NetDll_XNetGetConnectStatus(__in const IN_ADDR ina) {
#ifndef XLIVE_NO_NETDLL
	return XNET_CONNECT_STATUS_LOST;
#else
	return XNET_CONNECT_STATUS_LOST;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetDnsLookup(__in const char * pszHost, __in_opt WSAEVENT hEvent, __out XNDNS ** ppxndns) {
	if (ppxndns != NULL)
		*ppxndns = NULL;

#ifndef XLIVE_NO_NETDLL
	return WSAENETDOWN;
#else
	return WSAENETDOWN;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetDnsRelease(__in XNDNS * pxndns) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetQosListen(__in const XNKID * pxnkid,
										  __in_bcount_opt(cb) const BYTE * pb,
										  __in UINT cb,
										  __in DWORD dwBitsPerSec, __in DWORD dwFlags) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetQosLookup(__in UINT cxna,
										  __in_ecount_opt(cxna) const XNADDR * apxna[],
										  __in_ecount_opt(cxna) const XNKID * apxnkid[],
										  __in_ecount_opt(cxna) const XNKEY * apxnkey[],
										  __in UINT cina,
										  __in_ecount_opt(cina) const IN_ADDR aina[],
										  __in_ecount_opt(cina) const DWORD adwServiceId[],
										  __in UINT cProbes, __in DWORD dwBitsPerSec, __in DWORD dwFlags, __in_opt WSAEVENT hEvent, __out XNQOS ** ppxnqos) {
	if (ppxnqos != NULL)
		*ppxnqos = NULL;

#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetQosServiceLookup(__in DWORD dwFlags, __in_opt WSAEVENT hEvent, __deref_out XNQOS ** ppxnqos) {
	if (ppxnqos != NULL)
		*ppxnqos = NULL;

#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetQosRelease(__in XNQOS * pxnqos) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetQosGetListenStats(__in const XNKID * pxnkid, __inout XNQOSLISTENSTATS * pQosListenStats) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API DWORD WSAAPI NetDll_XNetGetTitleXnAddr(__out XNADDR * pxna) {
	pxna->ina.s_addr = ::htonl(INADDR_LOOPBACK);

#ifndef XLIVE_NO_NETDLL
	return XNET_GET_XNADDR_STATIC;
#else
	return XNET_GET_XNADDR_STATIC;
#endif
}

XLIVE_API DWORD WSAAPI NetDll_XNetGetDebugXnAddr(__out XNADDR * pxna) {
	pxna->ina.s_addr = ::htonl(INADDR_LOOPBACK);

#ifndef XLIVE_NO_NETDLL
	return XNET_GET_XNADDR_STATIC;
#else
	return XNET_GET_XNADDR_STATIC;
#endif
}

XLIVE_API DWORD WSAAPI NetDll_XNetGetEthernetLinkStatus() {
#ifndef XLIVE_NO_NETDLL
	return XNET_ETHERNET_LINK_ACTIVE;
#else
	return XNET_ETHERNET_LINK_ACTIVE;
#endif
}

XLIVE_API DWORD WSAAPI NetDll_XNetGetBroadcastVersionStatus(__in BOOL fReset) {
#ifndef XLIVE_NO_NETDLL
	return 0;
#else
	return 0;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetGetOpt(__in DWORD dwOptId, __out_bcount_opt( *pdwValueSize ) BYTE * pbValue, __inout DWORD * pdwValueSize) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetSetOpt(__in DWORD dwOptId, __in_bcount( dwValueSize ) const BYTE * pbValue, __in DWORD dwValueSize) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetStartupEx(DWORD XNCALLER_TYPE, __in_opt const XNetStartupParams * pxnsp, DWORD) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetReplaceKey(__in const XNKID * pxnkidUnregister, __in_opt const XNKID * pxnkidReplace) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetGetXnAddrPlatform(__in const XNADDR * pxnaddr, __out DWORD * pdwPlatform) {
	*pdwPlatform = XNET_XNADDR_PLATFORM_WINPC;

#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetGetSystemLinkPort(__out WORD * pwSystemLinkPort) {
	*pwSystemLinkPort = 0;

#ifndef XLIVE_NO_NETDLL
	return WSAEACCES;
#else
	return WSAEACCES;
#endif
}

XLIVE_API INT WSAAPI NetDll_XNetSetSystemLinkPort(__in WORD wSystemLinkPort) {
#ifndef XLIVE_NO_NETDLL
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}