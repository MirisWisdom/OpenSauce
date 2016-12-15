/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
#pragma once

// In release builds we don't want no shitty mcshit exception crap being included in the PE
#if NDEBUG || defined(YELO_KILL_CXX_EXCEPTIONS)
	#define _HAS_EXCEPTIONS 0

	bool __uncaught_exception();

// Should no longer need this after VS2008
//	#ifndef __cplusplus_cli
//		// http://stackoverflow.com/a/3870281
//		#define _STATIC_CPPLIB
//	#endif
	// We'll get a stupid fucking error in typeinfo.h about std::exception not being defined if we don't include it. Pretty sure this is a failing on Microshat's end. Was this fixed in 2010?
	#include <exception>
#endif