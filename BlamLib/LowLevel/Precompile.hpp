/*
    BlamLib: .NET SDK for the Blam Engine

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