/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

//#define XLIVE_NO_NETDLL
//#define XLIVE_NO_XAPI
//#define XLIVE_NO_XAM
#define XLIVE_NO_XLIVE

extern "C"
{
	#include "XLive/XLive.Xapi.hpp"
	#include "XLive/XLive.NetDll.hpp"

	typedef struct {
		DWORD SizeOfStruct;
		PAD32;
		PAD32;
		PAD32;
		PAD32;
		PAD32;
	}XLiveInitializeParams;
	BOOST_STATIC_ASSERT( sizeof(XLiveInitializeParams) == 0x18 );


	enum {
		eXUserSigninState_SignedInLocally = 1,


		XLIVE_FEIGN_PROTECTED_BUFFER_SIG = 0xDEADC0DE,
	};

	typedef struct {
		DWORD dwSignature;
		DWORD dwSize;
		PAD32;
		PAD32;
	}XLiveFeignProtectedBuffer;
};