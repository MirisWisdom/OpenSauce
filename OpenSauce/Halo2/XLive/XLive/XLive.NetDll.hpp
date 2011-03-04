/*
    Yelo: Open Sauce SDK

    Copyright (C) Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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