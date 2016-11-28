/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#define __EL_INCLUDE_FILE_MACRO(file)	\
	BOOST_PP_STRINGIZE( BOOST_PP_CAT(Memory/, BOOST_PP_CAT(PLATFORM_VERSION_VALUE, /##file) )

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
// 0x00 Open Sauce		__EL_<dir>_
#define __EL_INCLUDE_OPEN_SAUCE	0x00
	#define __EL_MEMORY_FUNCTION_INTERFACE	(__EL_INCLUDE_OPEN_SAUCE + 0)
	#define __EL_COMMON_GAME_SYSTEMS		(__EL_INCLUDE_OPEN_SAUCE + 1)
	#define __EL_COMMON_DEBUG_DUMP			(__EL_INCLUDE_OPEN_SAUCE + 2)


//////////////////////////////////////////////////////////////////////////
// 0x10 Game		__EL_GAME_
#define __EL_INCLUDE_GAME	0x10
	#define __EL_GAME_AI				(__EL_INCLUDE_GAME + 0)
	#define __EL_GAME_CAMERA			(__EL_INCLUDE_GAME + 1)
	#define __EL_GAME_CAMPAIGN			(__EL_INCLUDE_GAME + 2)
	#define __EL_GAME_CONSOLE			(__EL_INCLUDE_GAME + 3)
	#define __EL_GAME_EFFECTS			(__EL_INCLUDE_GAME + 4)
	#define __EL_GAME_ENGINE_FUNCTIONS	(__EL_INCLUDE_GAME + 5)
	#define __EL_GAME_GAME_ENGINE		(__EL_INCLUDE_GAME + 6)
	#define __EL_GAME_GAME_STATE		(__EL_INCLUDE_GAME + 7)
	#define __EL_GAME_LIGHTS			(__EL_INCLUDE_GAME + 8)
	#define __EL_GAME_PLAYERS			(__EL_INCLUDE_GAME + 9)
	#define __EL_GAME_SCRIPTING			(__EL_INCLUDE_GAME + 0xA)
	#define __EL_GAME_SCRIPT_LIBRARY	(__EL_INCLUDE_GAME + 0xB)
	#define __EL_GAME_BUILD_NUMBER		(__EL_INCLUDE_GAME + 0xC)


//////////////////////////////////////////////////////////////////////////
// 0x20 Interface	__EL_INTERFACE_
#define __EL_INCLUDE_INTERFACE	0x20
	#define __EL_INTERFACE_CONTROLS		(__EL_INCLUDE_INTERFACE + 0)
	#define __EL_INTERFACE_GAME_UI		(__EL_INCLUDE_INTERFACE + 1)
	#define __EL_INTERFACE_KEYSTONE		(__EL_INCLUDE_INTERFACE + 2)
	#define __EL_INTERFACE_UI_WIDGET	(__EL_INCLUDE_INTERFACE + 3)


//////////////////////////////////////////////////////////////////////////
// 0x30 Networking	__EL_NETWORKING_
#define __EL_INCLUDE_NETWORKING 0x30
	#define __EL_NETWORKING_MDP					(__EL_INCLUDE_NETWORKING + 0)
	#define __EL_NETWORKING_MESSAGE_DEFINITIONS (__EL_INCLUDE_NETWORKING + 1)
	#define __EL_NETWORKING_MESSAGE_DELTAS		(__EL_INCLUDE_NETWORKING + 2)
	#define __EL_NETWORKING_NETWORKING			(__EL_INCLUDE_NETWORKING + 3)
	#define __EL_NETWORKING_SERVER				(__EL_INCLUDE_NETWORKING + 4)
	#define __EL_NETWORKING_GAMESPY_API			(__EL_INCLUDE_NETWORKING + 5)


//////////////////////////////////////////////////////////////////////////
// 0x40 Objects		__EL_OBJECTS_
#define __EL_INCLUDE_OBJECTS	0x40
	#define __EL_OBJECTS_OBJECTS	(__EL_INCLUDE_OBJECTS + 0)
	#define __EL_OBJECTS_EQUIPMENT	(__EL_INCLUDE_OBJECTS + 1)
	#define __EL_OBJECTS_UNITS		(__EL_INCLUDE_OBJECTS + 2)


//////////////////////////////////////////////////////////////////////////
// 0x50 Rasterizer	__EL_RASTERIZER_
#define __EL_INCLUDE_RASTERIZER 0x50
	#define __EL_RASTERIZER_RASTERIZER	(__EL_INCLUDE_RASTERIZER + 0)
	#define __EL_RASTERIZER_LIGHTMAPS	(__EL_INCLUDE_RASTERIZER + 1)
	#define __EL_RASTERIZER_SKY			(__EL_INCLUDE_RASTERIZER + 2)

#define __EL_INCLUDE_RASTERIZER_DX9		(__EL_INCLUDE_RASTERIZER + 1)
	#define __EL_RASTERIZER_DX9_DX9			(__EL_INCLUDE_RASTERIZER_DX9 + 0)
	#define __EL_RASTERIZER_DX9_GBUFFER		(__EL_INCLUDE_RASTERIZER_DX9 + 1)
	#define __EL_RASTERIZER_DX9_VSHADERS	(__EL_INCLUDE_RASTERIZER_DX9 + 2)
	#define __EL_RASTERIZER_DX9_DEVICEHOOKS	(__EL_INCLUDE_RASTERIZER_DX9 + 3)

#define __EL_INCLUDE_RASTERIZER_SHADEREXTENSION	(__EL_INCLUDE_RASTERIZER + 3)
	#define __EL_RASTERIZER_SHADEREXTENSION				 (__EL_INCLUDE_RASTERIZER_SHADEREXTENSION + 0)
	#define __EL_RASTERIZER_SHADEREXTENSION_MODEL		 (__EL_INCLUDE_RASTERIZER_SHADEREXTENSION + 1)
	#define __EL_RASTERIZER_SHADEREXTENSION_ENVIRONMENT	 (__EL_INCLUDE_RASTERIZER_SHADEREXTENSION + 2)
	#define __EL_RASTERIZER_SHADEREXTENSION_EFFECT		 (__EL_INCLUDE_RASTERIZER_SHADEREXTENSION + 3)

#define __EL_INCLUDE_RASTERIZER_SHADERDRAW		(__EL_INCLUDE_RASTERIZER + 4)
	#define __EL_RASTERIZER_SHADERDRAW			(__EL_INCLUDE_RASTERIZER_SHADERDRAW + 0)


//////////////////////////////////////////////////////////////////////////
// 0x60 Scenario	__EL_SCENARIO_
#define __EL_INCLUDE_SCENARIO	0x60
#define __EL_SCENARIO_SCENARIO		(__EL_INCLUDE_SCENARIO + 0)


//////////////////////////////////////////////////////////////////////////
// 0x65 TagGroups	__EL_TAGGROUPS_
#define __EL_INCLUDE_TAGGROUPS	0x65
	#define __EL_TAGGROUPS_CACHE_FILES	(__EL_INCLUDE_TAGGROUPS + 0)
	#define __EL_TAGGROUPS_TAG_GROUPS	(__EL_INCLUDE_TAGGROUPS + 1)
