/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

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
	#define __EL_COMMON_YELO_SETTINGS		(__EL_INCLUDE_OPEN_SAUCE + 0)
	#define __EL_MEMORY_CHE_APE_INTERFACE	(__EL_INCLUDE_OPEN_SAUCE + 1)


//////////////////////////////////////////////////////////////////////////
// 0x10 Game		__EL_GAME_
#define __EL_INCLUDE_GAME	0x10
	#define __EL_GAME_ENGINE_FUNCTIONS	(__EL_INCLUDE_GAME + 0)


//////////////////////////////////////////////////////////////////////////
// 0x20 Interface	__EL_INTERFACE_
#define __EL_INCLUDE_INTERFACE	0x20


//////////////////////////////////////////////////////////////////////////
// 0x30 Networking	__EL_NETWORKING_
#define __EL_INCLUDE_NETWORKING 0x30


//////////////////////////////////////////////////////////////////////////
// 0x40 Objects		__EL_OBJECTS_
#define __EL_INCLUDE_OBJECTS	0x40


//////////////////////////////////////////////////////////////////////////
// 0x50 Rasterizer	__EL_RASTERIZER_
#define __EL_INCLUDE_RASTERIZER 0x50
	#define __EL_RASTERIZER_RASTERIZER	(__EL_INCLUDE_RASTERIZER + 0)

#define __EL_INCLUDE_RASTERIZER_DX9 (__EL_INCLUDE_RASTERIZER + 1)
	#define __EL_RASTERIZER_DX9_DX9		(__EL_INCLUDE_RASTERIZER_DX9 + 0)


//////////////////////////////////////////////////////////////////////////
// 0x60 TagGroups	__EL_TAGGROUPS_
#define __EL_INCLUDE_TAGGROUPS	0x60
	#define __EL_TAGGROUPS_CACHE_FILES	(__EL_INCLUDE_TAGGROUPS + 0)
	#define __EL_TAGGROUPS_TAG_GROUPS	(__EL_INCLUDE_TAGGROUPS + 1)

