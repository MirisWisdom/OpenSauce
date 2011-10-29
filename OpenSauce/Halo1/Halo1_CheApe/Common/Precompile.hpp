/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
// Precompiled header include

#pragma once

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