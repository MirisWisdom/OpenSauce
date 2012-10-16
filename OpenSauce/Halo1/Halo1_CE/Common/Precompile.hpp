/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#pragma once

#include <cseries/KillCxxExceptions.hpp>

#define WIN32_LEAN_AND_MEAN
#include "resource.h"
#include <windows.h>
#include <sal.h>

#include <WinSock2.h>

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

#define _USE_MATH_DEFINES // hurrrrrrrrrr, i like math!
#include <math.h>
#include <d3dx9math.h>

#if !PLATFORM_IS_DEDI
	#include <d3dx9.h>

	#define DIRECTINPUT_VERSION 0x0800
	#include <Dinput.h>
#endif

//////////////////////////////////////////////////////////////////////////
// STD C includes
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <io.h>
#include <errno.h>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// STL includes
#include <utility>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Boost includes
#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
//////////////////////////////////////////////////////////////////////////


#include <TinyXml/tinyxml.hpp>


//////////////////////////////////////////////////////////////////////////
// GameSpy includes & definitions

// Comment out this if you don't have access to the Open SDK
// You'll also need to remove the code file references from the project
#define YELO_USE_GAMESPY_OPEN

#if defined(YELO_USE_GAMESPY_OPEN)

	/*
		GameSpy SDK bug fix:
		There is a bug in GHTTP that causes HTTP redirects to fail if both
		the 'Content-Location' and 'Location' headers are returned by the server.

		Use the following on line 1695 in ghttpProcess.c to fix this:

			// Find the new location.
            /////////////////////////
            location = strstr(headers, "\r\nLocation:");// YELO: UPDATE THIS LINE
            if(location)
            {
                char * end;

                // Find the start of the URL.
                /////////////////////////////
                location += 11;							// YELO: UPDATE THIS LINE

	*/
//	BOOST_STATIC_ASSERT(false); // comment this out once the fix is applied

	// Just to make sure we're always using ASCII
	#undef GSI_UNICODE

	#include <GameSpyOpen/ghttp/ghttp.h>
	
	#define YELO_VERSION_CHECK_ENABLE

#endif
//////////////////////////////////////////////////////////////////////////

#if !defined(_DEBUG)
	#define YELO_NO_NETWORK
#endif

#include "Common/Platform.hpp"
#include <cseries/MacrosCpp.hpp>
#include <Common/BaseBlamTypes.hpp>
#include <Common/BaseBlamTypesPc.hpp>
#include <Blam/Halo1/BlamMemoryUpgrades.hpp>

#if defined(_DEBUG)
	// Mostly just useful for debug checking something (eg, game memory) on startup or on the first pass
	#define DebugRunOnce(...)							\
	{	static volatile bool iran_once_##__COUNTER__;	\
		if( iran_once_##__COUNTER__ == false )			\
		{												\
			iran_once_##__COUNTER__ = true;				\
			__VA_ARGS__	;								\
	} }
#else
	#define DebugRunOnce(...) __noop
#endif

namespace Yelo
{
	// Displays a message to the user using the WinAPI
	// Use this when are probably about to get really messy...
	// Fucking nasty location to put this function, but for the time being, it will do
	void PrepareToDropError(cstring text);
};

#include "Common/DebugFile.hpp"
#include "Common/DebugAssert.hpp"
#include "Common/YeloSettings.hpp"

#define HRESULT_ERETURN(p) if(FAILED(p)) return E_FAIL