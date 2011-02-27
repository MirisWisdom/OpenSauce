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
		XLIVE_FEIGN_PROTECTED_BUFFER_SIG = 0xDEADC0DE,
	};

	typedef struct {
		DWORD dwSignature;
		DWORD dwSize;
		PAD32;
		PAD32;
	}XLiveFeignProtectedBuffer;
};