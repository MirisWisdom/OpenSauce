/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#ifdef UTILDLL_EXPORTS
	#define UTILDLL_API __declspec(dllexport)
#else
	#define UTILDLL_API __declspec(dllimport)
#endif