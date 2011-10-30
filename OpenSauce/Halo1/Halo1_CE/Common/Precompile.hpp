/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

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

#if defined(NDEBUG)
	#define YELO_NO_NETWORK
#endif

#include "Common/Platform.hpp"
#include <cseries/MacrosCpp.hpp>
#include <Common/BaseBlamTypes.hpp>
#include <Common/BaseBlamTypesPc.hpp>
#include <Blam/Halo1/BlamMemoryUpgrades.hpp>

#include "Common/DebugFile.hpp"


#define HRESULT_ERETURN(p) if(FAILED(p)) return E_FAIL