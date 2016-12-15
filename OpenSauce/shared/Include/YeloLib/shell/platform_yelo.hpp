/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

//////////////////////////////////////////////////////////////////////////
// For all codebases
#define PLATFORM_TARGET_PC		0x00000001
// Xbox 1
#define PLATFORM_TARGET_XBOX	0x00000002
// Xbox 360
#define PLATFORM_TARGET_XENON	0x00000003

// Define the following in specific game codebases
//#define PLATFORM_TARGET		PLATFORM_TARGET_


//////////////////////////////////////////////////////////////////////////
// For game codebases
#define PLATFORM_GAME_CLIENT	0x00000000
#define PLATFORM_GAME_DEDI		0x00000001

// Define the following in specific game codebases
//#define PLATFORM_IS_EDITOR FALSE
//#define PLATFORM_IS_STUBBS FALSE

// Is the target platform user based? (aka, haloce.exe)
#define PLATFORM_IS_USER ( PLATFORM_TYPE == PLATFORM_GAME_CLIENT )
// Is the target platform dedicated based? (aka, haloceded.exe)
#define PLATFORM_IS_DEDI ( PLATFORM_TYPE == PLATFORM_GAME_DEDI )


//////////////////////////////////////////////////////////////////////////
// For editor codebases
#define PLATFORM_EDITOR_START_ID	0x00000002
#define PLATFORM_GUERILLA	(PLATFORM_EDITOR_START_ID+0)
#define PLATFORM_TOOL		(PLATFORM_EDITOR_START_ID+1)
#define PLATFORM_SAPIEN		(PLATFORM_EDITOR_START_ID+2)

#define CHEAPE_PLATFORM_HALO1 1
#define CHEAPE_PLATFORM_HALO2 2

// Define the following in specific editor codebases
//#define PLATFORM_IS_EDITOR TRUE
//#define CHEAPE_PLATFORM CHEAPE_PLATFORM_

// Set to zero to enable code blocks which are unused (eg, engine hooks which have no special code, yet, in OS)
#define PLATFORM_DISABLE_UNUSED_CODE 1


#if !defined(PLATFORM_TYPE)
	#if defined(PLATFORM_TYPE_CLIENT)
		#define PLATFORM_TYPE PLATFORM_GAME_CLIENT
	#elif defined(PLATFORM_TYPE_DEDI)
		#define PLATFORM_TYPE PLATFORM_GAME_DEDI
	#elif defined(PLATFORM_TYPE_GUERILLA)
		#define PLATFORM_TYPE PLATFORM_GUERILLA
	#elif defined(PLATFORM_TYPE_TOOL)
		#define PLATFORM_TYPE PLATFORM_TOOL
	#elif defined(PLATFORM_TYPE_SAPIEN)
		#define PLATFORM_TYPE PLATFORM_SAPIEN
	#else
		#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat. Are you selecting a build config that is just vanilla Debug or Release?
	#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Engine value markup system
//
// [game_value]		: value for use in Runtime (ie, Client or Dedi) builds
// [editor_value]	: value for use in Editor (ie, HEK) builds
//
//////////////////////////////////////////////////////////////////////////
#if PLATFORM_IS_EDITOR
	#define PLATFORM_ENGINE_VALUE(game_value, editor_value) editor_value
#else
	#define PLATFORM_ENGINE_VALUE(game_value, editor_value) game_value
#endif


// Current platform runs on tag files
#define PLATFORM_USES_TAG_FILES		(PLATFORM_IS_EDITOR==TRUE)
// Current platform runs on cache files
#define PLATFORM_USES_CACHE_FILES	(PLATFORM_IS_EDITOR==FALSE)


// TODO: this needs to be relocated. Had to put it here when we migrated to static libs, since the static libs and DLL code all reference it
#define ASSERTS_ENABLED