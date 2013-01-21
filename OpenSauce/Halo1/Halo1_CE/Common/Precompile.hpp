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

#define ASSERTS_ENABLED
//#define API_DEBUG_MEMORY
#if PLATFORM_IS_DEDI
	#define API_NO_7ZIP_CODEC
	#define API_NO_ZIP_CODEC
#endif

#include "Common/Platform.hpp"

#include <blamlib/Halo1/cseries/cseries.hpp>
#include <blamlib/scenario/scenario_location.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include <YeloLib/cseries/errors_yelo.hpp>
#include "Common/YeloSettings.hpp"