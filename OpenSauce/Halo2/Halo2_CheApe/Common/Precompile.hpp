/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

//#define _WIN32_WINNT 0x0400 // wtf, why do I even need to define this??
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define _USE_MATH_DEFINES // hurrrrrrrrrr, i like math!
#include <math.h>
#include <d3dx9math.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>

#include <direct.h>

// qsort
#include <stdlib.h>
#include <search.h>

#include <TinyXml/tinyxml.hpp>

#include <cseries/MacrosCpp.hpp>
#include <Common/BaseBlamTypes.hpp>
#include <Common/BaseBlamTypesPc.hpp>
#include "Common/Platform.hpp"
#include "Common/BlamTypes.hpp"

#include <Common/CheApeShared.hpp>

#include <Memory/Halo2/DataShared.hpp>
#include <Memory/MemoryInterfaceShared.hpp>

#ifdef _DEBUG
	#define MSG_BOX(title, msg) MessageBoxW(NULL, msg, title, MB_OK)
#else
	#define MSG_BOX(title, msg) __noop
#endif

#if defined(_DEBUG) || defined(_TRACE)
	#define TRACE_BOX(title, msg) MessageBoxW(NULL, msg, title, MB_OK)
#else
	#define TRACE_BOX(title, msg) __noop
#endif