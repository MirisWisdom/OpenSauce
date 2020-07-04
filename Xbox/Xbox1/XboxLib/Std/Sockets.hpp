/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

// ******************************************************************
// * Networking
// ******************************************************************
	//XBOX_WINPROC(SOCKET, accept, (
	//			 IN SOCKET s,
	//			 OUT struct sockaddr FAR * addr,
	//			 IN OUT int FAR * addrlen
	//			 ));

	XBOX_WINPROC(int, bind, (
				 IN SOCKET s,
				 IN const struct sockaddr FAR * name,
				 IN int namelen
				 ));
	#define Std_bind XboxStd.bind

	XBOX_WINPROC(int, closesocket, (
				 IN SOCKET s
				 ));
	#define Std_closesocket XboxStd.closesocket

	XBOX_WINPROC(int, connect, (
				 IN SOCKET s,
				 IN const struct sockaddr FAR * name,
				 IN int namelen
				 ));
	#define Std_connect XboxStd.connect

	XBOX_WINPROC(int, ioctlsocket, (
				 IN SOCKET s,
				 IN long cmd,
				 IN OUT u_long FAR * argp
				 ));
	#define Std_ioctlsocket XboxStd.ioctlsocket

	//XBOX_WINPROC(int, getpeername, (
	//			 IN SOCKET s,
	//			 OUT struct sockaddr FAR * name,
	//			 IN OUT int FAR * namelen
	//			 ));

	//XBOX_WINPROC(int, getsockname, (
	//			 IN SOCKET s,
	//			 OUT struct sockaddr FAR * name,
	//			 IN OUT int FAR * namelen
	//			 ));

	XBOX_WINPROC(int, getsockopt, (
				 IN SOCKET s,
				 IN int level,
				 IN int optname,
				 OUT char FAR * optval,
				 IN OUT int FAR * optlen
				 ));
	#define Std_getsockopt XboxStd.getsockopt

	//XBOX_WINPROC(u_long, htonl, (
	//			 IN u_long hostlong
	//			 ));

	XBOX_WINPROC(u_short, htons, (
				 IN u_short hostshort
				 ));
	#define Std_htons XboxStd.htons

	XBOX_WINPROC(unsigned long, inet_addr, (
				 IN const char FAR * cp
				 ));
	#define Std_inet_addr XboxStd.inet_addr

	//XBOX_WINPROC(int, listen, (
	//			 IN SOCKET s,
	//			 IN int backlog
	//			 ));

	XBOX_WINPROC(u_long, ntohl, (
				 IN u_long netlong
				 ));
	#define Std_ntohl XboxStd.ntohl

	//XBOX_WINPROC(u_short, ntohs, (
	//			 IN u_short netshort
	//			 ));

	XBOX_WINPROC(int, recv, (
				 IN SOCKET s,
				 OUT char FAR * buf,
				 IN int len,
				 IN int flags
				 ));
	#define Std_recv XboxStd.recv

	XBOX_WINPROC(int, recvfrom, (
				 IN SOCKET s,
				 OUT char FAR * buf,
				 IN int len,
				 IN int flags,
				 OUT struct sockaddr FAR * from,
				 IN OUT int FAR * fromlen
				 ));
	#define Std_recvfrom XboxStd.recvfrom

	XBOX_WINPROC(int, select, (
				 IN int nfds,
				 IN OUT fd_set FAR * readfds,
				 IN OUT fd_set FAR * writefds,
				 IN OUT fd_set FAR *exceptfds,
				 IN const struct timeval FAR * timeout
				 ));
	#define Std_select XboxStd.select

	XBOX_WINPROC(int, send, (
				 IN SOCKET s,
				 IN const char FAR * buf,
				 IN int len,
				 IN int flags
				 ));
	#define Std_send XboxStd.send

	XBOX_WINPROC(int, sendto, (
				 IN SOCKET s,
				 IN const char FAR * buf,
				 IN int len,
				 IN int flags,
				 IN const struct sockaddr FAR * to,
				 IN int tolen
				 ));
	#define Std_sendto XboxStd.sendto

	XBOX_WINPROC(int, setsockopt, (
				 IN SOCKET s,
				 IN int level,
				 IN int optname,
				 IN const char FAR * optval,
				 IN int optlen
				 ));
	#define Std_setsockopt XboxStd.setsockopt

	XBOX_WINPROC(int, shutdown, (
				 IN SOCKET s,
				 IN int how
				 ));
	#define Std_shutdown XboxStd.shutdown

	XBOX_WINPROC(SOCKET, socket, (
				 IN int af,
				 IN int type,
				 IN int protocol
				 ));
	#define Std_socket XboxStd.socket