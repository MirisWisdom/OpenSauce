/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <YeloLib/shell/platform_yelo.hpp>
#define PLATFORM_TARGET		PLATFORM_TARGET_XBOX
#define PLATFORM_IS_EDITOR	FALSE


#define PLATFORM_H2			0x00000001
// The last Auto-Update
#define PLATFORM_H2_AU		0x00000002
#define PLATFORM_H2_ALPHA	0x00000003

#define PLATFORM_ID			PLATFORM_H2

#if !defined(PLATFORM_ID)
	#if defined(PLATFORM_TYPE_RETAIL)
		#define PLATFORM_ID PLATFORM_H2
	#elif defined(PLATFORM_TYPE_RETAIL_AU)
		#define PLATFORM_ID PLATFORM_H2_AU
	#elif defined(PLATFORM_TYPE_BETA)
		#define PLATFORM_ID PLATFORM_H2_ALPHA
	#else
		#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat.
	#endif
#endif

// Calling convention of Halo 2 Xbox
#define PLATFORM_API __stdcall


// The Halo 2 alpha was the only game build which still used debug data in the tag interface structures
#if PLATFORM_ID != PLATFORM_H2_ALPHA
	#define PLATFORM_USE_CONDENSED_TAG_INTERFACE 1
#endif


// Platform conditional code macros
// NOTE: so far there is no need for a PLATFORM_CODE_H2_AU macro
#if PLATFORM_ID == PLATFORM_H2
	#define PLATFORM_CODE_H2_ALPHA( ... )
	#define PLATFORM_CODE_H2( ... )			__VA_ARGS__

#elif PLATFORM_ID == PLATFORM_H2_ALPHA
	#define PLATFORM_CODE_H2_ALPHA( ... )	__VA_ARGS__
	#define PLATFORM_CODE_H2( ... )
#endif


// Uncomment for beta testing builds
//#define PLATFORM_BETA