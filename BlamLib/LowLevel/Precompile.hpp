/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once
#include <Windows.h>
#include <shell/Platform.hpp>
#include <cseries/MacrosCpp.hpp>
#include <cseries/MacrosClr.hpp>

#ifdef _DEBUG
// Force no C++ exceptions
#define YELO_KILL_CXX_EXCEPTIONS

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <cseries/KillCxxExceptions.hpp>
#endif

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>



// Path to the 360 SDK
#define PATH_XEDK PATH_PROGRAM_FILES "\\Microsoft Xbox 360 SDK"
#define PATH_XEDK_LIBS	PATH_XEDK "\\lib\\win32\\vs2010\\"

#ifndef LOWLEVEL_NO_X360
	#ifdef _DEBUG
		#pragma comment (lib, PATH_XEDK_LIBS "d3d9d.lib")
		#pragma comment (lib, PATH_XEDK_LIBS "d3dx9d.lib")
		#pragma comment (lib, PATH_XEDK_LIBS "xgraphicsd.lib")
		#pragma comment (lib, PATH_XEDK_LIBS "xcompressd.lib")
	#else
		#pragma comment (lib, PATH_XEDK_LIBS "d3d9.lib")
		#pragma comment (lib, PATH_XEDK_LIBS "d3dx9.lib")
		#pragma comment (lib, PATH_XEDK_LIBS "xgraphics.lib")
		#pragma comment (lib, PATH_XEDK_LIBS "xcompress.lib")
	#endif
#else
	// If LOWLEVEL_NO_X360 is defined, implicitly kill the XMA lib code
	#define LOWLEVEL_NO_X360_XMA

	#pragma comment (lib, "d3d9.lib")
	#pragma comment (lib, "d3dx9.lib")
#endif

// Library's function convention
#define API_FUNC __stdcall