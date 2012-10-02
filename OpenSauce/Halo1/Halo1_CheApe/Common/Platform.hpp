/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <Common/PlatformShared.hpp>
#define PLATFORM_TARGET		PLATFORM_TARGET_PC
#define PLATFORM_IS_EDITOR	TRUE
#define CHEAPE_PLATFORM		CHEAPE_PLATFORM_HALO1

// PLATFORM_TYPE is defined in the vcproj build configurations
#if !defined(PLATFORM_ID)
	#if defined(PLATFORM_TYPE_GUERILLA)
		#define PLATFORM_ID PLATFORM_GUERILLA
	#elif defined(PLATFORM_TYPE_TOOL)
		#define PLATFORM_ID PLATFORM_TOOL
	#elif defined(PLATFORM_TYPE_SAPIEN)
		#define PLATFORM_ID PLATFORM_SAPIEN
	#else
		#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat. Are you selecting a build config that is just vanilla Debug or Release?
	#endif
#endif

// Calling convention of guerilla\tool\sapien
#define PLATFORM_API __cdecl


// See this header for more "Engine pointer markup system" documentation
#include <Memory/MemoryInterfaceShared.hpp>

//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
// [g_addr]	: Guerilla memory address
// [t_addr]	: Tool memory address
// [s_addr]	: Sapien memory address
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_ID == PLATFORM_GUERILLA
	#define PLATFORM_VALUE(g_value, t_value, s_value) g_value
#elif PLATFORM_ID == PLATFORM_TOOL
	#define PLATFORM_VALUE(g_value, t_value, s_value) t_value
#elif PLATFORM_ID == PLATFORM_SAPIEN
	#define PLATFORM_VALUE(g_value, t_value, s_value) s_value
#endif

#define ENGINE_DPTR(type, name, g_addr, t_addr, s_addr) \
	static type##** const pp##name = CAST_PTR(type##**, PLATFORM_VALUE(g_addr,t_addr,s_addr));								BOOST_STATIC_ASSERT( PLATFORM_VALUE(g_addr,t_addr,s_addr) != NULL );

#define ENGINE_PTR(type, name, g_addr, t_addr, s_addr) \
	static type##* const p##name = CAST_PTR(type##*, PLATFORM_VALUE(g_addr,t_addr,s_addr));									BOOST_STATIC_ASSERT( PLATFORM_VALUE(g_addr,t_addr,s_addr) != NULL );

#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = PLATFORM_VALUE(g_addr,t_addr,s_addr) };	BOOST_STATIC_ASSERT( GET_FUNC_PTR(name) != NULL );
#define DATA_PTR(name, g_addr, t_addr, s_addr) enum DATA_PTR_##name { PTR_##name = PLATFORM_VALUE(g_addr,t_addr,s_addr) };	BOOST_STATIC_ASSERT( GET_DATA_PTR(name) != NULL );