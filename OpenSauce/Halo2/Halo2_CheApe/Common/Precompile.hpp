/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

#include <cseries/KillCxxExceptions.hpp>

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


#include "Common/Platform.hpp"

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>
#undef ASSERT // TODO: H2's YELO_ASSERT isn't compatible with H1's
#define ASSERT(value, message) YELO_ASSERT(_error_category_debug, value)

#include <YeloLib/open_sauce/che_ape/che_ape_api.hpp>

#include <blamlib/Halo2/memory/data.hpp> // gotta do this here b/c tag_groups_base references s_data_iterator

#include "Memory/MemoryInterface.hpp"

#ifdef _DEBUG
	#define MSG_BOX(title, msg) MessageBoxW(nullptr, msg, title, MB_OK)
#else
	#define MSG_BOX(title, msg) __noop
#endif

#if defined(_DEBUG) || defined(_TRACE)
	#define TRACE_BOX(title, msg) MessageBoxW(nullptr, msg, title, MB_OK)
#else
	#define TRACE_BOX(title, msg) __noop
#endif