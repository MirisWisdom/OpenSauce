/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

XLIVE_API int WSAAPI NetDll_WSAStartup(IN WORD wVersionRequested, OUT LPWSADATA lpWSAData) {
#ifndef XLIVE_NO_NETDLL
	return ::WSAStartup(wVersionRequested, lpWSAData);
#else
	lpWsaData->wVersion = 2;
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_WSACleanup() {
#ifndef XLIVE_NO_NETDLL
	return ::WSACleanup();
#else
	return NO_ERROR;
#endif
}

XLIVE_API SOCKET WSAAPI NetDll_socket(IN int af, IN int type, IN int protocol) {
#ifndef XLIVE_NO_NETDLL
	return ::socket(af, type, protocol);
#else
	return INVALID_SOCKET;
#endif
}

XLIVE_API int WSAAPI NetDll_closesocket(IN SOCKET s) {
#ifndef XLIVE_NO_NETDLL
	return ::closesocket(s);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_shutdown(IN SOCKET s, IN int how) {
#ifndef XLIVE_NO_NETDLL
	return ::shutdown(s, how);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_ioctlsocket(IN SOCKET s, IN long cmd, __inout u_long FAR * argp) {
#ifndef XLIVE_NO_NETDLL
	return ::ioctlsocket(s, cmd, argp);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_setsockopt(IN SOCKET s, IN int level,
									   IN int optname,
									   __in_bcount_opt(optlen) const char FAR * optval,
									   IN int optlen) {
#ifndef XLIVE_NO_NETDLL
	return ::setsockopt(s, level, optname, optval, optlen);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_getsockopt(IN SOCKET s, IN int level,
									   IN int optname,
									   __out_bcount(*optlen) char FAR * optval,
									   __inout int FAR * optlen) {
#ifndef XLIVE_NO_NETDLL
	return ::getsockopt(s, level, optname, optval, optlen);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_getsockname(IN SOCKET s,
										__out_bcount_part(*namelen,*namelen) struct sockaddr FAR * name,
										__inout int FAR * namelen) {
#ifndef XLIVE_NO_NETDLL
	return ::getsockname(s, name, namelen);
#else
	if (namelen != NULL && name != NULL && *namelen == sizeof (sockaddr_in)) 
		memset(name, 0, sizeof(sockaddr_in));

	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_getpeername(IN SOCKET s,
										__out_bcount_part(*namelen,*namelen) struct sockaddr FAR * name,
										__inout int FAR * namelen) {
#ifndef XLIVE_NO_NETDLL
	return ::getpeername(s, name, namelen);
#else
	if (namelen != NULL && name != NULL && *namelen == sizeof (sockaddr_in)) 
		memset(name, 0, sizeof(sockaddr_in));

	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_bind(IN SOCKET s,
								 __in_bcount(namelen) const struct sockaddr FAR * name,
								 IN int namelen) {
#ifndef XLIVE_NO_NETDLL
	return ::bind(s, name, namelen);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_connect(IN SOCKET s,
									__in_bcount(namelen) const struct sockaddr FAR * name,
									IN int namelen) {
#ifndef XLIVE_NO_NETDLL
	return ::connect(s, name, namelen);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_listen(IN SOCKET s, IN int backlog) {
#ifndef XLIVE_NO_NETDLL
	return ::listen(s, backlog);
#else
	return NO_ERROR;
#endif
}

XLIVE_API SOCKET WSAAPI NetDll_accept(IN SOCKET s,
								   __out_bcount_opt(*addrlen) struct sockaddr FAR * addr,
								   __inout_opt int FAR * addrlen) {
#ifndef XLIVE_NO_NETDLL
	return ::accept(s, addr, addrlen);
#else
	return INVALID_SOCKET;
#endif
}

XLIVE_API int WSAAPI NetDll_select(IN int nfds,
								   __inout_opt fd_set FAR * readfds,
								   __inout_opt fd_set FAR * writefds,
								   __inout_opt fd_set FAR * exceptfds,
								   IN const struct timeval FAR * timeout) {
#ifndef XLIVE_NO_NETDLL
	return ::select(nfds, readfds, writefds, exceptfds, timeout);
#else
	return NO_ERROR;
#endif
}

XLIVE_API BOOL WSAAPI NetDll_WSAGetOverlappedResult(IN SOCKET s,
													IN LPWSAOVERLAPPED lpOverlapped,
													OUT LPDWORD lpcbTransfer,
													IN BOOL fWait,
													OUT LPDWORD lpdwFlags) {
#ifndef XLIVE_NO_NETDLL
	return WSAGetOverlappedResult(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
#else
	return FALSE;
#endif
}

XLIVE_API int WSAAPI NetDll_WSACancelOverlappedIO(IN SOCKET s) {
	return NO_ERROR;
}

XLIVE_API int WSAAPI NetDll_recv(IN SOCKET s,
								 __out_bcount_part(len, return) __out_data_source(NETWORK) char FAR * buf,
								 IN int len,
								 IN int flags) {
#ifndef XLIVE_NO_NETDLL
	return ::recv(s, buf, len, flags);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_WSARecv(IN SOCKET s,
									__in_ecount(dwBufferCount) __out_data_source(NETWORK) LPWSABUF lpBuffers,
									IN DWORD dwBufferCount,
									__out_opt LPDWORD lpNumberOfBytesRecvd,
									IN OUT LPDWORD lpFlags,
									__in_opt LPWSAOVERLAPPED lpOverlapped,
									__in_opt LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
#ifndef XLIVE_NO_NETDLL
	return ::WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_recvfrom(IN SOCKET s,
									 __out_bcount_part(len, return) __out_data_source(NETWORK) char FAR * buf,
									 IN int len,
									 IN int flags,
									 __out_bcount_opt(*fromlen) struct sockaddr FAR * from,
									 __inout_opt int FAR * fromlen) {
#ifndef XLIVE_NO_NETDLL
	return ::recvfrom(s, buf, len, flags, from, fromlen);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_WSARecvFrom(IN SOCKET s,
										__in_ecount(dwBufferCount) __out_data_source(NETWORK) LPWSABUF lpBuffers,
										IN DWORD dwBufferCount,
										__out_opt LPDWORD lpNumberOfBytesRecvd,
										IN OUT LPDWORD lpFlags,
										__out_bcount_part_opt(*lpFromlen,*lpFromLen) struct sockaddr FAR * lpFrom,
										__inout_opt LPINT lpFromlen,
										__in_opt LPWSAOVERLAPPED lpOverlapped,
										__in_opt LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
#ifndef XLIVE_NO_NETDLL
	return ::WSARecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_send(IN SOCKET s,
								 __in_bcount(len) const char FAR * buf,
								 IN int len, IN int flags) {
#ifndef XLIVE_NO_NETDLL
	return ::send(s, buf, len, flags);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_WSASend(IN SOCKET s,
									__in_ecount(dwBufferCount) LPWSABUF lpBuffers,
									IN DWORD dwBufferCount,
									__out_opt LPDWORD lpNumberOfBytesSent,
									IN DWORD dwFlags,
									__in_opt LPWSAOVERLAPPED lpOverlapped,
									__in_opt LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
#ifndef XLIVE_NO_NETDLL
	return ::WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_sendto(IN SOCKET s,
								   __in_bcount(len) const char FAR * buf,
								   IN int len,
								   IN int flags,
								   __in_bcount(tolen) const struct sockaddr FAR * to,
								   IN int tolen) {
#ifndef XLIVE_NO_NETDLL
	return ::sendto(s, buf, len, flags, to, tolen);
#else
	return NO_ERROR;
#endif
}

XLIVE_API int WSAAPI NetDll_WSASendTo(IN SOCKET s,
									  __in_ecount(dwBufferCount) LPWSABUF lpBuffers,
									  IN DWORD dwBufferCount,
									  __out_opt LPDWORD lpNumberOfBytesSent,
									  IN DWORD dwFlags,
									  __in_bcount(iTolen) const struct sockaddr FAR * lpTo,
									  IN int iTolen,
									  __in_opt LPWSAOVERLAPPED lpOverlapped,
									  __in_opt LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
#ifndef XLIVE_NO_NETDLL
	return ::WSASendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine);
#else
	return NO_ERROR;
#endif
}

XLIVE_API unsigned long WSAAPI NetDll_inet_addr(__in IN const char FAR * cp) {
#ifndef XLIVE_NO_NETDLL
	return ::inet_addr(cp);
#else
	return INADDR_NONE;
#endif
}

XLIVE_API int WSAAPI NetDll_WSAGetLastError() {
#ifndef XLIVE_NO_NETDLL
	return ::WSAGetLastError();
#else
	return NO_ERROR;
#endif
}

XLIVE_API void WSAAPI NetDll_WSASetLastError(IN int iError) {
#ifndef XLIVE_NO_NETDLL
	::WSASetLastError(iError);
#else
	// Do nothing
#endif
}

XLIVE_API WSAEVENT WSAAPI NetDll_WSACreateEvent() {
#ifndef XLIVE_NO_NETDLL
	return ::WSACreateEvent();
#else
	return WSA_INVALID_EVENT;
#endif
}

XLIVE_API BOOL WSAAPI NetDll_WSACloseEvent(IN WSAEVENT hEvent) {
#ifndef XLIVE_NO_NETDLL
	return ::WSACloseEvent(hEvent);
#else
	return TRUE;
#endif
}

XLIVE_API BOOL WSAAPI NetDll_WSASetEvent(IN WSAEVENT hEvent) {
#ifndef XLIVE_NO_NETDLL
	return ::WSASetEvent(hEvent);
#else
	return TRUE;
#endif
}

XLIVE_API BOOL WSAAPI NetDll_WSAResetEvent(IN WSAEVENT hEvent) {
#ifndef XLIVE_NO_NETDLL
	return ::WSAResetEvent(hEvent);
#else
	return TRUE;
#endif
}

XLIVE_API DWORD WSAAPI NetDll_WSAWaitForMultipleEvents(IN DWORD cEvents, IN const WSAEVENT FAR * lphEvents,
													 IN BOOL fWaitAll, IN DWORD dwTimeout,  IN BOOL fAlertable) {
#ifndef XLIVE_NO_NETDLL
	return ::WSAWaitForMultipleEvents(cEvents, lphEvents, fWaitAll, dwTimeout, fAlertable);
#else
	return WSA_WAIT_FAILED;
#endif
}

XLIVE_API int WSAAPI NetDll___WSAFDIsSet(SOCKET fd, fd_set FAR * set) {
#ifndef XLIVE_NO_NETDLL
	return ::__WSAFDIsSet(fd, set);
#else
	return 0;
#endif
}

XLIVE_API int WSAAPI NetDll_WSAEventSelect(IN SOCKET s, IN WSAEVENT hEventObject, IN long lNetworkEvents) {
#ifndef XLIVE_NO_NETDLL
	return ::WSAEventSelect(s, hEventObject, lNetworkEvents);
#else
	return NO_ERROR;
#endif
}

XLIVE_API u_long WSAAPI NetDll_htonl(IN u_long hostlong) {
	return ::htonl(hostlong);
}

XLIVE_API u_short WSAAPI NetDll_ntohs(IN u_short netshort) {
	return ::ntohs(netshort);
}

XLIVE_API u_long WSAAPI NetDll_ntohl(IN u_long netlong) {
	return ::ntohl(netlong);
}

XLIVE_API u_short WSAAPI NetDll_htons(IN u_short hostshort) {
	return ::htons(hostshort);
}