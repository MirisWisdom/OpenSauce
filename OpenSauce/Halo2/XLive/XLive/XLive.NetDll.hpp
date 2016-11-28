/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

typedef struct {
	BYTE cfgSizeOfStruct;
	BYTE cfgFlags;
	BYTE cfgSockMaxDgramSockets;
	BYTE cfgSockMaxStreamSockets;
	BYTE cfgSockDefaultRecvBufsizeInK;
	BYTE cfgSockDefaultSendBufsizeInK;
	BYTE cfgKeyRegMax;
	BYTE cfgSecRegMax;
	BYTE cfgQosDataLimitDiv4;
	BYTE cfgQosProbeTimeoutInSeconds;
	BYTE cfgQosProbeRetries;
	BYTE cfgQosSrvMaxSimultaneousResponses;
	BYTE cfgQosPairWaitTimeInSeconds;
}XNetStartupParams;

typedef struct {
	IN_ADDR ina;
	IN_ADDR inaOnline;
	WORD wPortOnline;
	BYTE abEnet[6];
	BYTE abOnline[20];
}XNADDR, TSADDR;

typedef struct {
	BYTE        ab[8];
}XNKID;

typedef struct {
	BYTE        ab[16];
}XNKEY;

typedef struct {
	INT         iStatus;
	UINT        cina;
	IN_ADDR     aina[8];
}XNDNS;

typedef struct {
	BYTE bFlags;
	BYTE bReserved;
	WORD cProbesXmit;
	BYTE cProbesRecv;
	WORD cbData;
	BYTE *pbData;
	WORD wRttMinInMsecs;
	WORD wRttMedInMsecs;
	DWORD dwUpBitsPerSec;
	DWORD dwDnBitsPerSec;
}XNQOSINFO;

typedef struct {
	UINT cxnqos;
	UINT cxnqosPending;
	XNQOSINFO axnqosinfo[1];
}XNQOS;

typedef struct {
	DWORD dwSizeOfStruct;
	DWORD dwNumDataRequestsReceived;
	DWORD dwNumProbesReceived;
	DWORD dwNumSlotsFullDiscards;
	DWORD dwNumDataRepliesSent;
	DWORD dwNumProbeRepliesSent;
}XNQOSLISTENSTATS;

enum {
	XNET_XNADDR_PLATFORM_XBOX1		= 0,
	XNET_XNADDR_PLATFORM_XBOX360,
	XNET_XNADDR_PLATFORM_WINPC,

	XNET_CONNECT_STATUS_IDLE		= 0,
	XNET_CONNECT_STATUS_PENDING,
	XNET_CONNECT_STATUS_CONNECTED,
	XNET_CONNECT_STATUS_LOST,

	XNET_GET_XNADDR_STATIC			= 4,

	XNET_ETHERNET_LINK_ACTIVE		= 1<<0,
};


typedef struct {
	XNKID sessionID;
	XNADDR hostAddress;
	XNKEY keyExchangeKey;
}XSESSION_INFO, *PXSESSION_INFO;