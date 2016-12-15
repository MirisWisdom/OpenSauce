/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
/*!
 * Microsoft Visual C++ compiler warning disabling
 */
#pragma once

//#define _CRT_SECURE_NO_DEPRECATE

// 'reinterpret_cast' : pointer truncation
#pragma warning(disable: 4311)

// 'type cast' : conversion from '<type>' to '<type> *' of greater size
#pragma warning(disable: 4312)

// macro redefinition
// used to get rid of the fucking warning in CSeries/CppMacros.h
// for the interface macro (complains in file objbase.h, a
// system include)
#pragma warning(disable: 4005)