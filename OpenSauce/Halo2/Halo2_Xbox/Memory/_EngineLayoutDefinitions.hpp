/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#if PLATFORM_ID == PLATFORM_H2_ALPHA
	#define PLATFORM_RESOURCE_FOLDER	0.Alpha
#elif PLATFORM_ID == PLATFORM_H2
	#define PLATFORM_RESOURCE_FOLDER	1.00
#elif PLATFORM_ID == PLATFORM_H2_AU
	#define PLATFORM_RESOURCE_FOLDER	1.AU
#endif

#define __EL_INCLUDE_FILE_MACRO(file)	\
	BOOST_PP_STRINGIZE( BOOST_PP_CAT(Memory/, BOOST_PP_CAT(PLATFORM_RESOURCE_FOLDER, /##file) )

// __EL_INCLUDE_ID format:		__EL_<dir>
// __EL_INCLUDE_FILE_ID format: __EL_<dir>_<file>
//#define __EL__

// NOTE:
// __EL_INCLUDE_ID can't go higher than 0x100, because this breaks the
// BOOST_PP_ITERATION limitations (actually BOOST_PP_INC limitations).
//
// if __EL_INCLUDE_ID is 0x101 or higher, any conditions will just be 
// ignored and any data defined under those conditions won't (of course)
// be compiled, causing 'X was undefined' errors

//////////////////////////////////////////////////////////////////////////
// 0x00 Open Sauce			__EL_<dir>_
#define __EL_INCLUDE_OPEN_SAUCE		0x00
	#define __EL_COMMON_PRECOMPILE	(__EL_INCLUDE_OPEN_SAUCE + 0)

//////////////////////////////////////////////////////////////////////////
// 0x04 AI			__EL_AI_
#define __EL_INCLUDE_AI				0x04
	#define __EL_AI_AI	(__EL_INCLUDE_AI + 0)


//////////////////////////////////////////////////////////////////////////
// 0x0A Cache		__EL_CACHE_
#define __EL_INCLUDE_CACHE			0x0A
	#define __EL_CACHE_CACHE		(__EL_INCLUDE_CACHE + 0)
	#define __EL_CACHE_XBOX_CACHE	(__EL_INCLUDE_CACHE + 1)


//////////////////////////////////////////////////////////////////////////
// 0x10 Effects		__EL_EFFECTS_
#define __EL_INCLUDE_EFFECTS		0x10
	#define __EL_EFFECTS_EFFECTS	(__EL_INCLUDE_EFFECTS + 0)


//////////////////////////////////////////////////////////////////////////
// 0x1A Game		__EL_GAME_
#define __EL_INCLUDE_GAME				0x1A
	#define __EL_GAME_DIRECTOR			(__EL_INCLUDE_GAME + 0)
	#define __EL_GAME_ENGINE_FUNCTIONS	(__EL_INCLUDE_GAME + 1)
	#define __EL_GAME_GAME_ENGINE		(__EL_INCLUDE_GAME + 2)
	#define __EL_GAME_GAME_TEAMS		(__EL_INCLUDE_GAME + 3)
	#define __EL_GAME_GLOBALS			(__EL_INCLUDE_GAME + 4)
	#define __EL_GAME_MAIN				(__EL_INCLUDE_GAME + 5)
	#define __EL_GAME_PLAYERS			(__EL_INCLUDE_GAME + 6)
	#define __EL_GAME_RASTERIZER		(__EL_INCLUDE_GAME + 7)


//////////////////////////////////////////////////////////////////////////
// 0x2A HS			__EL_HS_
#define __EL_INCLUDE_HS	0x2A
	#define __EL_HS_HS	(__EL_INCLUDE_HS + 0)


//////////////////////////////////////////////////////////////////////////
// 0x30 Interface	__EL_INTERFACE_
#define __EL_INCLUDE_INTERFACE			0x30
	#define __EL_INTERFACE_HUD			(__EL_INCLUDE_INTERFACE + 0)
	#define __EL_INTERFACE_INPUT		(__EL_INCLUDE_INTERFACE + 1)
	#define __EL_INTERFACE_INTERFACE	(__EL_INCLUDE_INTERFACE + 2)


//////////////////////////////////////////////////////////////////////////
// 0x3A Models		__EL_MODELS_


//////////////////////////////////////////////////////////////////////////
// 0x40 Networking	__EL_NETWORKING_
#define __EL_INCLUDE_NETWORKING			0x40
	#define __EL_NETWORKING_NETWORKING	(__EL_INCLUDE_NETWORKING + 0)
	#define __EL_NETWORKING_SIMULATION	(__EL_INCLUDE_NETWORKING + 1)
	#define __EL_NETWORKING_STATISTICS	(__EL_INCLUDE_NETWORKING + 2)


//////////////////////////////////////////////////////////////////////////
// 0x4A Objects		__EL_OBJECTS_
#define __EL_INCLUDE_OBJECTS		0x4A
	#define __EL_OBJECTS_OBJECTS	(__EL_INCLUDE_OBJECTS + 0)
	#define __EL_OBJECTS_WIDGETS	(__EL_INCLUDE_OBJECTS + 1)


//////////////////////////////////////////////////////////////////////////
// 0x50 Physics		__EL_PHYSICS_
#define __EL_INCLUDE_PHYSICS		0x50
	#define __EL_PHYSICS_PHYSICS	(__EL_INCLUDE_PHYSICS + 0)


//////////////////////////////////////////////////////////////////////////
// 0x5A Scenario	__EL_SCENARIO_


//////////////////////////////////////////////////////////////////////////
// 0x60 Shaders		__EL_SHADERS_


//////////////////////////////////////////////////////////////////////////
// 0x6A Sound		__EL_SOUND_
#define __EL_INCLUDE_SOUND		0x6A
	#define __EL_SOUND_SOUND	(__EL_INCLUDE_SOUND + 0)


//////////////////////////////////////////////////////////////////////////
// 0x70 Structures	__EL_STRUCTURES_
#define __EL_INCLUDE_STRUCTURES				0x70
	#define __EL_STRUCTURES_STRUCTURE_BSP	(__EL_INCLUDE_STRUCTURES + 0)


//////////////////////////////////////////////////////////////////////////
// 0x7A ?		__EL_?_