/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdlib.h>

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>

#include <cseries/MacrosCpp.hpp>

#include <Winsock2.h>

#ifdef XLIVE_EXPORTS
	#define XLIVE_API __declspec(dllexport)
#else
	#define XLIVE_API __declspec(dllimport)
#endif