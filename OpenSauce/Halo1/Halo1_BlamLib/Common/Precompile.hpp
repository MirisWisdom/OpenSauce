/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#pragma once

#include "Common/targetver.h"

#include <cseries/KillCxxExceptions.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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

#include <vld.h>

//////////////////////////////////////////////////////////////////////////
// STD C includes
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <io.h>
#include <errno.h>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// STL includes
#include <array>
#include <iterator>
#include <memory> // std::unique_ptr
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Boost includes
#if API_DEBUG
	#define BOOST_LIB_DIAGNOSTIC
#endif

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>

#include <boost/integer/static_log2.hpp>

#include <boost/filesystem.hpp>
//////////////////////////////////////////////////////////////////////////


#include "Common/Platform.hpp"

#include <blamlib/Halo1/cseries/cseries.hpp>
#include <blamlib/scenario/scenario_location.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include <YeloLib/cseries/errors_yelo.hpp>
#include <blamlib/Halo1/cseries/errors.hpp>
#include <YeloLib/Halo1/cseries/memory_yelo.hpp>
