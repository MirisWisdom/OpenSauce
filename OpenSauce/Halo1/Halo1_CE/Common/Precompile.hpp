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

#include "Common/DebugFile.hpp"
#include "Common/DebugAssert.hpp"


#define HRESULT_ERETURN(p) if(FAILED(p)) return E_FAIL

namespace Yelo
{
	// Displays a message to the user using the WinAPI
	// Use this when are probably about to get really messy...
	// Fucking nasty location to put this function, but for the time being, it will do
	void PrepareToDropError(cstring text);
};