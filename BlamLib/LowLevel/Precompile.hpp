/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

// Force no C++ exceptions since we always build LowLevel as Debug (even for releases)
#define YELO_KILL_CXX_EXCEPTIONS
#include <cseries/KillCxxExceptions.hpp>

// Path to the 360 SDK
#define PATH_XEDK PATH_PROGRAM_FILES "\\Microsoft Xbox 360 SDK"

#ifndef LOWLEVEL_NO_X360
	#ifdef _DEBUG
		#pragma comment (lib, PATH_XEDK "\\lib\\win32\\vs2008\\d3d9d.lib")
		#pragma comment (lib, PATH_XEDK "\\lib\\win32\\vs2008\\d3dx9d.lib")
		#pragma comment (lib, PATH_XEDK "\\lib\\win32\\vs2008\\xgraphicsd.lib")
	#else
		#pragma comment (lib, PATH_XEDK "\\lib\\win32\\vs2008\\d3d9.lib")
		#pragma comment (lib, PATH_XEDK "\\lib\\win32\\vs2008\\d3dx9.lib")
		#pragma comment (lib, PATH_XEDK "\\lib\\win32\\vs2008\\xgraphics.lib")
	#endif
#else
	// If LOWLEVEL_NO_X360 is defined, implicitly kill the XMA lib code
	#define LOWLEVEL_NO_X360_XMA

	#pragma comment (lib, "d3d9.lib")
	#pragma comment (lib, "d3dx9.lib")
#endif

#include <Windows.h>
#include <shell/Platform.hpp>
#include <cseries/MacrosCpp.hpp>
#include <cseries/MacrosClr.hpp>

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Library's function convention
#define API_FUNC __stdcall