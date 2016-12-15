/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <cseries/KillCxxExceptions.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// Only because we end up including the various bnet network transport headers
#include <WinSock2.h>

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

#define _USE_MATH_DEFINES // hurrrrrrrrrr, i like math!
#include <math.h>
#include <d3dx9math.h>

#include <vld.h>

//////////////////////////////////////////////////////////////////////////
// STD C includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <io.h>

// _mkdir
#include <direct.h>

// qsort
#include <stdlib.h>
#include <search.h>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// STL includes
#include <array>
#include <iterator>
#include <memory> // std::unique_ptr
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>
#include <functional>
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Boost includes
#if API_DEBUG
	#define BOOST_LIB_DIAGNOSTIC
#endif
#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>

#include <boost/integer/static_log2.hpp>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
//////////////////////////////////////////////////////////////////////////


#include <TinyXml/tinyxml.hpp>


#include "Common/Platform.hpp"
#if PLATFORM_TYPE == PLATFORM_GUERILLA
	#define PLATFORM_NO_DX9
#endif

#include <blamlib/Halo1/cseries/cseries.hpp>
#include <blamlib/scenario/scenario_location.hpp>
#include <YeloLib/cseries/errors_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include <YeloLib/open_sauce/che_ape/che_ape_api.hpp>

#ifndef PLATFORM_NO_DX9
	#include <d3dx9.h>
	#include <d3d9.h>
#endif

#include "Settings/Settings.hpp"
#include "Memory/MemoryInterface.hpp"

#ifdef _DEBUG
	#define MSG_BOX(title, msg) MessageBoxA(nullptr, msg, title, MB_OK)
#else
	#define MSG_BOX(title, msg) __noop
#endif

#if defined(_DEBUG) || defined(_TRACE)
	#define TRACE_BOX(title, msg) MessageBoxA(nullptr, msg, title, MB_OK)
#else
	#define TRACE_BOX(title, msg) __noop
#endif
