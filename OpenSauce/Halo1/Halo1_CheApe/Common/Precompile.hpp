/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <cseries/KillCxxExceptions.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

#define _USE_MATH_DEFINES // hurrrrrrrrrr, i like math!
#include <math.h>
#include <d3dx9math.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>

// _mkdir
#include <direct.h>

// qsort
#include <stdlib.h>
#include <search.h>


// C++ STL
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>

//#include <boost/filesystem.hpp>


#include <TinyXml/tinyxml.hpp>

#include <cseries/MacrosCpp.hpp>
#include <Common/BaseBlamTypes.hpp>
#include <Common/BaseBlamTypesPc.hpp>


#include "Common/Platform.hpp"
#if PLATFORM_ID == PLATFORM_GUERILLA
	#define PLATFORM_NO_DX9
#endif

#include <Common/CheApeShared.hpp>

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include <Memory/Halo1/DataShared.hpp>
#include <Memory/MemoryInterfaceShared.hpp>


#ifndef PLATFORM_NO_DX9
	#include <d3dx9.h>
	#include <d3d9.h>
#endif


#ifdef _DEBUG
	#define MSG_BOX(title, msg) MessageBoxA(NULL, msg, title, MB_OK)
#else
	#define MSG_BOX(title, msg) __noop
#endif

#if defined(_DEBUG) || defined(_TRACE)
	#define TRACE_BOX(title, msg) MessageBoxA(NULL, msg, title, MB_OK)
#else
	#define TRACE_BOX(title, msg) __noop
#endif