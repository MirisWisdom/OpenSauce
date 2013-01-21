/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/shell/platform_yelo.hpp>
#define PLATFORM_TARGET		PLATFORM_TARGET_PC
#define PLATFORM_IS_EDITOR	FALSE

//////////////////////////////////////////////////////////////////////////
//#define PLATFORM_TYPE 0x00??

// PLATFORM_TYPE is defined in the vcproj build configurations
#if !defined(PLATFORM_TYPE)
	#if defined(PLATFORM_TYPE_CLIENT)
		#define PLATFORM_TYPE 0x0000
	#elif defined(PLATFORM_TYPE_DEDI)
		#define PLATFORM_TYPE 0x0001
	#else
		#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat.
	#endif
#endif

// Is the target platform user based? (aka, haloce.exe)
#define PLATFORM_IS_USER ( PLATFORM_TYPE == 0x0000 )
// Is the target platform dedicated based? (aka, haloceded.exe)
#define PLATFORM_IS_DEDI ( PLATFORM_TYPE == 0x0001 )
//////////////////////////////////////////////////////////////////////////


#if !defined(PLATFORM_VERSION)
	//#define PLATFORM_VERSION 0x1080 // 1.08 is no longer actively supported, use at your own risk
	#define PLATFORM_VERSION 0x1090 // latest supported release of haloce
#endif


#if PLATFORM_VERSION == 0x1080
	#define PLATFORM_VERSION_VALUE 1.08
	#define PLATFORM_VERSION_BUILD 0616
#elif PLATFORM_VERSION == 0x1090
	#define PLATFORM_VERSION_VALUE 1.09
	#define PLATFORM_VERSION_BUILD 0620
#else
	#error FUCK YOU, this version isn't supported
#endif

// Target platform identifier
#define PLATFORM_ID ( (PLATFORM_TYPE<<16) | PLATFORM_VERSION )


// Calling convention of blam engine
#define PLATFORM_API __cdecl


// Name of the section where we'll place executable data
#define API_CODEDATA_SECTION_NAME	".yelo"
// Apply this to data which will later contain code which will also be executed
#define API_CODEDATA				__declspec(allocate(API_CODEDATA_SECTION_NAME))
// The fucking *compiler* ignores 'write' with 'execute. Needs manual fix-up
#pragma section( API_CODEDATA_SECTION_NAME, read,write,execute )
// ...Actually, this fixes the problem of the compiler ignoring the 'write' attribute
// When compiled into object-code (.obj), the 'write' attribute is lost, so it's not really the linker's fault
#pragma comment(linker, "/section:" API_CODEDATA_SECTION_NAME ",ERW")